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

int resolvehelper(const char* hostname, int family, const char* service, sockaddr_storage* pAddr)
{
    int result;
    addrinfo* result_list = NULL;
    addrinfo hints = {};
    hints.ai_family = family;
    hints.ai_socktype = SOCK_DGRAM; // without this flag, getaddrinfo will return 3x the number of addresses (one for each socket type).
    result = getaddrinfo(hostname, service, &hints, &result_list);
    if (result == 0)
    {
        //ASSERT(result_list->ai_addrlen <= sizeof(sockaddr_in));
        memcpy(pAddr, result_list->ai_addr, result_list->ai_addrlen);
        freeaddrinfo(result_list);
    }

    return result;
}

void error( char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv){
    srand(time(NULL));
    interface_receive.createSocketReceiveState(&global_state, "tcp://localhost:5555");  //! VSS-Vision e VSS-Simulator abrem um socket na porta 5555
	interface_send.createSendCommandsTeam1(&global_commands, "tcp://localhost:5556");   //! Team2 abre o socket na porta 5557
	interface_debug.createSendDebugTeam1(&global_debug, "tcp://localhost:5558");       //! Team2 abre o socket na porta 5559

    // Configuração de envio de dados para o LabView

    int result = 0;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    float s1, s2;
	float* s;	

    sockaddr_in addrListen = {};

    result = 0;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addrListen = {};

    addrListen.sin_family = AF_INET;
    result = bind(sock, (sockaddr*)&addrListen, sizeof(addrListen));
    if (result == -1)
    {
        int lasterror = errno;
        std::cout << "error: " << lasterror;
        // exit(1);
    }

    sockaddr_storage addrDest = {};
    result = resolvehelper("192.168.0.103", AF_INET, "9000", &addrDest);
    if (result != 0)
    {
        int lasterror = errno;
        std::cout << "error: " << lasterror;
        // exit(1);
    }

    // Fim de configuração de envio de dados para o LabView

    // Configuração de recebimento de dados do LabView

    int sockfd;
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in serv,client;
 
    serv.sin_family = AF_INET;
    serv.sin_port = htons(9000);
    serv.sin_addr.s_addr = inet_addr("192.168.0.103");

    char buffer[256];
    socklen_t l = sizeof(client);
 
    // Fim de configuração de recebimento de dados do LabView

    while(true){
        s = receive_state();
        send_commands();
        //send_debug();


        // Envio de dados

        s1 = s[0];
        s2 = s[1];

        string resultado1 = to_string(s1);
        string resultado2 = to_string(s2);
		string resultado = resultado1+resultado2;
        const char* msg = resultado.c_str();
        size_t msg_length = strlen(msg);

        result = sendto(sock, msg, msg_length, 0, (sockaddr*)&addrDest, sizeof(addrDest));

        // Fim de envio de dados

        // Recebimento de dados

        int rc= recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&client,&l);
        if(rc<0)
        {
            cout<<"ERROR READING FROM SOCKET";
        }
        cout<<"\n the message received is : "<<buffer<<endl;

        // Fim do recebimento de dados
        
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

        global_state.robots_blue(i).pose().x();             // Pos X do robô azul i
        global_state.robots_blue(i).pose().y();             // Pos Y do robô azul i
        global_state.robots_blue(i).pose().yaw();           // Rot Z do robô azul i

        global_state.robots_blue(i).v_pose().x();           // Vel X do robô azul i
        global_state.robots_blue(i).v_pose().y();           // Vel Y do robô azul i
        global_state.robots_blue(i).v_pose().yaw();         // Vel Z do robô azul i
    }

	current[0] = global_state.robots_yellow(1).pose().x();
	current[1] = global_state.robots_yellow(1).pose().y();
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
