	/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include "cstdlib"
#include "interface.h"

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "viewer.pb.h"
// #include "labview.pb.h"

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

	float* s;

    struct sockaddr_in addrF;
    addrF.sin_family = AF_INET;
    inet_aton("192.168.0.100", &addrF.sin_addr);
    addrF.sin_port = htons(9000);
    int sockF = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in addrE;
    addrE.sin_family = AF_INET;
    inet_aton("192.168.0.100", &addrE.sin_addr);
    addrE.sin_port = htons(9001);
    int sockE = socket(PF_INET, SOCK_DGRAM, 0);

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    Viewer::DataFriend viewerFriend;
    Viewer::DataEnemy viewerEnemy;

    std::string bufFriend;
    std::string bufEnemy;

 	while(true){

        s = receive_state();

        viewerFriend.set_ballposex(s[0]);
        viewerFriend.set_ballposey(s[1]);
        viewerFriend.set_friend1posex(s[2]);
        viewerFriend.set_friend1posey(s[3]);
        viewerFriend.set_friend1yaw(s[4]);
        viewerFriend.set_friend2posex(s[5]);
        viewerFriend.set_friend2posey(s[6]);
        viewerFriend.set_friend2yaw(s[7]);
        viewerFriend.set_friend3posex(s[8]);
        viewerFriend.set_friend3posey(s[9]);
        viewerFriend.set_friend3yaw(s[10]);

        viewerEnemy.set_enemy1posex(s[11]);
        viewerEnemy.set_enemy1posey(s[12]);
        viewerEnemy.set_enemy2posex(s[13]);
        viewerEnemy.set_enemy2posey(s[14]);
        viewerEnemy.set_enemy3posex(s[15]);
        viewerEnemy.set_enemy3posey(s[16]);

        viewerFriend.SerializeToString(&bufFriend);
        viewerEnemy.SerializeToString(&bufEnemy);

        sendto(sockF, bufFriend.data(), 65, 0, (struct sockaddr *)&addrF, sizeof(addrF)); // Friend data is sent via port 9000
        sendto(sockE, bufEnemy.data(), 30, 0, (struct sockaddr *)&addrE, sizeof(addrE)); // Enemy data is sent via port 9001

        bufFriend.clear();
        bufEnemy.clear();

        send_commands();

	}       	

	return 0;
}


float* receive_state(){
    interface_receive.receiveState();           //! Bloqueante até o VSS-Vision ou VSS-Simulator enviar um estado

	float* result;
	float current[17];
/*
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
*/
    current[0] = global_state.balls(0).pose().x();           // Pos X da bola
    current[1] = global_state.balls(0).pose().y();           // Pos Y da bola
	current[2] = global_state.robots_yellow(0).pose().x();           // Pos X do robô amarelo i
    current[3] = global_state.robots_yellow(0).pose().y();           // Pos Y do robô amarelo i
    current[4] = global_state.robots_yellow(0).pose().yaw();         // Rot Z do robô amarelo i
    current[5] = global_state.robots_yellow(1).pose().x();           // Pos X do robô amarelo i
    current[6] = global_state.robots_yellow(1).pose().y();           // Pos Y do robô amarelo i
    current[7] = global_state.robots_yellow(1).pose().yaw();         // Rot Z do robô amarelo i
    current[8] = global_state.robots_yellow(2).pose().x();           // Pos X do robô amarelo i
    current[9] = global_state.robots_yellow(2).pose().y();           // Pos Y do robô amarelo i
    current[10] = global_state.robots_yellow(2).pose().yaw();         // Rot Z do robô amarelo i

    current[11] = global_state.robots_blue(0).pose().x();             // Pos X do robô azul i
    current[12] = global_state.robots_blue(0).pose().y();             // Pos Y do robô azul i
    current[13] = global_state.robots_blue(1).pose().x();             // Pos X do robô azul i
    current[14] = global_state.robots_blue(1).pose().y();             // Pos Y do robô azul i
    current[15] = global_state.robots_blue(2).pose().x();             // Pos X do robô azul i
    current[16] = global_state.robots_blue(2).pose().y();             // Pos Y do robô azul i

	result = current;
    return result;
}

void send_commands(){
    global_commands = vss_command::Global_Commands();       // Devemos limpar, pois funciona como um vector
	global_commands.set_is_team_yellow(true);

	for(int i = 0 ; i < 3 ; i++){                           // todos os robôs vão girar
		vss_command::Robot_Command *robot = global_commands.add_robot_commands();
		robot->set_id(i);
		robot->set_left_vel(100*i);
		robot->set_right_vel(50*i);
		if (i==0){
			robot->set_left_vel(10);
			robot->set_right_vel(50);
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
