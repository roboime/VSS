	/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include "cstdlib"
#include "interface.h"

#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>

using namespace std;

Interface interface_receive;                    //! Interface de recebimento de estados
vss_state::Global_State global_state;           //! Pacote que define um estado

Interface interface_send;                       //! Interface de envio de comandos
vss_command::Global_Commands global_commands;   //! Pacote que define um comando

Interface interface_debug;                      //! Interface de envio de informações de debug visual
vss_debug::Global_Debug global_debug;           //! Pacote que define o debug visual

float* receive_state();
void send_commands();
void send_debug();


int main(int argc, char** argv){
    srand(time(NULL));
    interface_receive.createSocketReceiveState(&global_state, "tcp://localhost:5555");  //! VSS-Vision e VSS-Simulator abrem um socket na porta 5555
	interface_send.createSendCommandsTeam1(&global_commands, "tcp://localhost:5556");   //! Team2 abre o socket na porta 5557
	interface_debug.createSendDebugTeam1(&global_debug, "tcp://localhost:5558");       //! Team2 abre o socket na porta 5559

	float s0,s1; // ,s2; //s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,s16;
	float* s;

	int server_fd, client;
	struct sockaddr_in address, addc;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[544];

	if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if ((client = socket(AF_INET, SOCK_DGRAM, 0)) == 0){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}


	if (setsockopt(client, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(9000);


	addc.sin_family = AF_INET;
	addc.sin_addr.s_addr = inet_addr("192.168.0.101");
	addc.sin_port = htons(8000);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

 	while(true){

		recvfrom(server_fd, buffer, sizeof(buffer), 0, (sockaddr *)&address, (socklen_t *)&addrlen);
		fflush(stdout);

        s = receive_state();
        send_commands();
        //send_debug();

        s0 = s[0];
        s1 = s[1];
        // s2 = s[2];
        
        /*s3 = s[3];
        s4 = s[4];
        s5 = s[5];
        s6 = s[6];
        s7 = s[7];
        s8 = s[8];
        s9 = s[9];
        s10 = s[10];
        s11 = s[11];
        s12 = s[12];
        s13 = s[13];
        s14 = s[14];
        s15 = s[15];
        s16 = s[16];
*/
        string resultado0 = to_string(s0);
        string resultado1 = to_string(s1);
		// string resultado2 = to_string(s2);
        /*string resultado3 = to_string(s3);
        string resultado4 = to_string(s4);
        string resultado5 = to_string(s5);
        string resultado6 = to_string(s6);
        string resultado7 = to_string(s7);
        string resultado8 = to_string(s8);
        string resultado9 = to_string(s9);
        string resultado10 = to_string(s10);
        string resultado11 = to_string(s11);
        string resultado12 = to_string(s12);
        string resultado13 = to_string(s13);
        string resultado14 = to_string(s14);
        string resultado15 = to_string(s15);
        string resultado16 = to_string(s16);
*/

		string resultado = resultado0+" "+resultado1; //+" "+resultado2; //+" "+resultado3+" "+resultado4+" "+resultado5+" "+resultado6+" "+resultado7+" "+resultado8+" "+resultado9+" "+resultado10+" "+resultado11+" "+resultado12+" "+resultado13+" "+resultado14+" "+resultado15+" "+resultado16;
        const char* msg = resultado.c_str();
        size_t msg_length = strlen(msg);

        sendto(client, msg, msg_length, 0, (sockaddr *)&addc, sizeof(addc));

	}       	

	return 0;
}


float* receive_state(){
    interface_receive.receiveState();           //! Bloqueante até o VSS-Vision ou VSS-Simulator enviar um estado

	float* result;
	float current[2];

	global_state.balls(0).pose().x();           // Pos X da bola
    global_state.balls(0).pose().y();           // Pos Y da bola

    global_state.balls(0).v_pose().x();         // Vel X da bola
    global_state.balls(0).v_pose().y();         // Vel Y da bola

    for(int i = 0 ; i < 3 ; i++){
        global_state.robots_yellow(i).pose().x();           // Pos X do robô amarelo i
        global_state.robots_yellow(i).pose().y();           // Pos Y do robô amarelo i
        global_state.robots_yellow(i).pose().yaw();         // Rot Z do robô amarelo i

        global_state.robots_yellow(i).v_pose().x();         // Vel X do robô amarelo i
        global_state.robots_yellow(i).v_pose().y();         // Vel Y do robô amarelo i
        global_state.robots_yellow(i).v_pose().yaw();       // Vel Z do robô amarelo i

     }

     for(int i = 0; i < 3; i++){

        global_state.robots_blue(i).pose().x();             // Pos X do robô azul i
        global_state.robots_blue(i).pose().y();             // Pos Y do robô azul i
        global_state.robots_blue(i).pose().yaw();           // Rot Z do robô azul i

        global_state.robots_blue(i).v_pose().x();           // Vel X do robô azul i
        global_state.robots_blue(i).v_pose().y();           // Vel Y do robô azul i
        global_state.robots_blue(i).v_pose().yaw();         // Vel Z do robô azul i
    
    }


    current[0] = global_state.balls(0).pose().x();           // Pos X da bola
    //current[1] = global_state.balls(0).pose().y();           // Pos Y da bola
	current[1] = global_state.robots_yellow(0).pose().x();           // Pos X do robô amarelo i
    //current[3] = global_state.robots_yellow(0).pose().y();           // Pos Y do robô amarelo i
    //current[4] = global_state.robots_yellow(0).pose().yaw();         // Rot Z do robô amarelo i
    //current[5] = global_state.robots_yellow(1).pose().x();           // Pos X do robô amarelo i
    //current[6] = global_state.robots_yellow(1).pose().y();           // Pos Y do robô amarelo i
    //current[7] = global_state.robots_yellow(1).pose().yaw();         // Rot Z do robô amarelo i
    //current[8] = global_state.robots_yellow(2).pose().x();           // Pos X do robô amarelo i
    //current[9] = global_state.robots_yellow(2).pose().y();           // Pos Y do robô amarelo i
    //current[10] = global_state.robots_yellow(2).pose().yaw();         // Rot Z do robô amarelo i

    //current[11] = global_state.robots_blue(0).pose().x();             // Pos X do robô azul i
    //current[12] = global_state.robots_blue(0).pose().y();             // Pos Y do robô azul i
    //current[13] = global_state.robots_blue(1).pose().x();             // Pos X do robô azul i
    //current[14] = global_state.robots_blue(1).pose().y();             // Pos Y do robô azul i
    //current[15] = global_state.robots_blue(2).pose().x();             // Pos X do robô azul i
    //current[16] = global_state.robots_blue(2).pose().y();             // Pos Y do robô azul i

	result = current; 
    return result;
}

void send_commands(){
    global_commands = vss_command::Global_Commands();       // Devemos limpar, pois funciona como um vector
	global_commands.set_is_team_yellow(true);

	for(int i = 0 ; i < 3 ; i++){                           // todos os robôs vão girar
		vss_command::Robot_Command *robot = global_commands.add_robot_commands();
		robot->set_id(i);
		robot->set_left_vel(15*i);
		robot->set_right_vel(40*i);
		if (i==0){
			robot->set_left_vel(10);
			robot->set_right_vel(10);
		}
	}

    interface_send.sendCommandTeam1();                      // Não bloqueante, envia os comandos para o VSS-Simulator
    // NOTE que aqui entraria o código de envio para robôs reais
}

void send_debug(){
	global_debug = vss_debug::Global_Debug();                       // Devemos limpar, pois funciona como um vector

	for(int i = 0 ; i < 3 ; i++){                                   // Todos os robôs terão o vetor de movimentação variado perto deles mesmos
		vss_debug::Pose *steps = global_debug.add_step_poses();
		steps->set_id(i);
		steps->set_x(global_state.robots_yellow(i).pose().x() - 10 + rand()%20);
		steps->set_y(global_state.robots_yellow(i).pose().y() - 10 + rand()%20);
		steps->set_yaw(global_state.robots_yellow(i).pose().yaw());
	}

	for(int i = 0 ; i < 3 ; i++){                                   // Todos os robôs terão a pose final variada perto da bola
		vss_debug::Pose *finals = global_debug.add_final_poses();
		finals->set_id(i);
		finals->set_x(global_state.balls(0).pose().x() - 10 + rand()%20);
		finals->set_y(global_state.balls(0).pose().y() - 10 + rand()%20);
		finals->set_yaw(0);
	}

	for(int i = 0 ; i < 3 ; i++){                                    // Todos os robôs teram um caminho que leva para a bola
		vss_debug::Path *paths = global_debug.add_paths();
		paths->set_id(i);

        // Pose do robô
        vss_debug::Pose *poses1 = paths->add_poses();
        poses1->set_id(i);
        poses1->set_x(global_state.robots_yellow(i).pose().x());
        poses1->set_y(global_state.robots_yellow(i).pose().y());
        poses1->set_yaw(0);

        // Pose da bola
        vss_debug::Pose *poses2 = paths->add_poses();
        poses2->set_id(i);
        poses2->set_x(global_state.balls(0).pose().x());
        poses2->set_y(global_state.balls(0).pose().y());
        poses2->set_yaw(0);
	}

	interface_debug.sendDebugTeam1();
}
