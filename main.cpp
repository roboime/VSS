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
#include <unistd.h>

#include <ctime>
#include <vector>

#include "viewer.pb.h"
// #include "labview.pb.h"

Interface interface_receive;                    //! Interface de recebimento de estados
vss_state::Global_State global_state;           //! Pacote que define um estado

Interface interface_send;                       //! Interface de envio de comandos
vss_command::Global_Commands global_commands;   //! Pacote que define um comando

Interface interface_debug;                      //! Interface de envio de informações de debug visual
vss_debug::Global_Debug global_debug;           //! Pacote que define o debug visual

vector<float> receive_state();
void send_commands(int *vel);
void send_debug();


int main(int argc, char** argv){
    srand(time(NULL));
    interface_receive.createSocketReceiveState(&global_state, "tcp://localhost:5555");  //! VSS-Vision e VSS-Simulator abrem um socket na porta 5555
	interface_send.createSendCommandsTeam1(&global_commands, "tcp://localhost:5556");   //! Team2 abre o socket na porta 5557
	interface_debug.createSendDebugTeam1(&global_debug, "tcp://localhost:5558");       //! Team2 abre o socket na porta 5559

	vector <float> s;

	// Envio dos pacotes dos amigos e da bola
    struct sockaddr_in addrF;
    addrF.sin_family = AF_INET;
    inet_aton("192.168.0.108", &addrF.sin_addr);
    addrF.sin_port = htons(9000);
   	int sockF = socket(PF_INET, SOCK_DGRAM, 0);

    // Envio dos pacotes dos inimigos
    struct sockaddr_in addrE;
    addrE.sin_family = AF_INET;
    inet_aton("192.168.0.108", &addrE.sin_addr);
    addrE.sin_port = htons(9001);
	int sockE = socket(PF_INET, SOCK_DGRAM, 0);
/*
    // Recebimento dos pacotes
	struct sockaddr_in addrR;
    addrR.sin_family = AF_INET;
    inet_aton("192.168.0.100", &addrR.sin_addr);
    addrR.sin_port = htons(9002);
    int sockR = socket(PF_INET, SOCK_DGRAM, 0);
    bind(sockR, (struct sockaddr *)&addrR, sizeof(addrR));
    socklen_t sizeaddR = sizeof(addrR);
    struct timeval read_timeout;
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 10;
    setsockopt(sockR, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);
*/

	struct sockaddr_in myaddr;
	struct sockaddr_in remaddr;
	socklen_t addrlen = sizeof(remaddr);
	//int recvlen;
	int fd;
	//char buf[24];

	if((fd = socket(AF_INET, SOCK_DGRAM, 0))<0){
		cout << "cannot create socket" << endl;
		return 0;
	}

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(9002);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr))<0){
		cout << "bind failed" << endl;
		return 0;
	}

	// struct timeval tv;
	// tv.tv_sec = 0;
	// tv.tv_usec = 150000;
	// if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))<0){
	// 	perror("Error");
	// }

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    Viewer::DataFriend viewerFriend;
    Viewer::DataEnemy viewerEnemy;
    Viewer::DataReceiver viewerReceiver;

    std::string bufFriend;
    std::string bufEnemy;
    //std::string bufReceiver;
    char bufReceiver[18];
    int sendVel[6];

 	while(true){

 		clock_t begin = clock();
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
/*
        recvfrom(sockR, &bufReceiver, 30, 0, (sockaddr *)&addrR, &sizeaddR);

        cout << bufReceiver << endl;

       	viewerReceiver.ParseFromString(bufReceiver);

       	cout << viewerReceiver.friend0veldir() << endl;
*/
        recvfrom(fd, &bufReceiver, 18, 0, (struct sockaddr *)&remaddr, &addrlen);
        viewerReceiver.ParseFromString(bufReceiver);
        // cout << viewerReceiver.friend0veldir() << endl;
        // cout << viewerReceiver.friend1veldir() << endl;
        // cout << viewerReceiver.friend2veldir() << endl;
        // cout << viewerReceiver.friend0velesq() << endl;
        // cout << viewerReceiver.friend1velesq() << endl;
        // cout << viewerReceiver.friend2velesq() << endl;

        sendVel[0] = viewerReceiver.friend0veldir() - 256;
        sendVel[1] = viewerReceiver.friend1veldir() - 256;
        sendVel[2] = viewerReceiver.friend2veldir() - 256;
        sendVel[3] = viewerReceiver.friend0velesq() - 256;
        sendVel[4] = viewerReceiver.friend1velesq() - 256;
        sendVel[5] = viewerReceiver.friend2velesq() - 256;

        cout << sendVel[0] << endl;

        send_commands(sendVel);
        memset(bufReceiver, 0, sizeof(bufReceiver));
        memset(sendVel, 0, sizeof(sendVel));

        clock_t end = clock();
        clock_t delta = end - begin;
        cout << delta/(double)CLOCKS_PER_SEC*1000 << endl;
	}       	

	return 0;
}


vector<float> receive_state(){
    interface_receive.receiveState();           //! Bloqueante até o VSS-Vision ou VSS-Simulator enviar um estado
    vector<float> result;
	float current[17];
/*

	INFORMAÇÕES QUE CONSIGO EXTRAIR DO GLOBAL_STATE

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


    for (int i = 0 ; i < 17 ; ++i){
    	result.emplace_back(current[i]);
    }
    return result;
}

void send_commands(int *vel){
    global_commands = vss_command::Global_Commands();       // Devemos limpar, pois funciona como um vector
	global_commands.set_is_team_yellow(true);
	int aux[6];
	aux[0] = vel[0];
	aux[1] = vel[1];
	aux[2] = vel[2];
	aux[3] = vel[3];
	aux[4] = vel[4];
	aux[5] = vel[5];

	for(int i = 0 ; i < 3 ; i++){                           // todos os robôs vão girar
		vss_command::Robot_Command *robot = global_commands.add_robot_commands();
		robot->set_id(i);
		robot->set_left_vel(aux[i]);
		robot->set_right_vel(aux[i+3]);
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
