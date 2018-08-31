/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>
#include <Communications/DebugSender.h>
#include "cstdlib"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <ctime>
#include <vector>

#include "viewer.pb.h"

using namespace vss;

IStateReceiver *stateReceiver;
ICommandSender *commandSender;
IDebugSender *debugSender;

State state;


void send_commands(int *vel);
void send_debug();

int main(int argc, char** argv){
    srand(time(NULL));

    stateReceiver = new StateReceiver();
    commandSender = new CommandSender();
    debugSender = new DebugSender();

    stateReceiver->createSocket();
    commandSender->createSocket(TeamType::Yellow);
    debugSender->createSocket(TeamType::Yellow);

    std::vector <float> s;

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

    struct sockaddr_in myaddr;
    struct sockaddr_in remaddr;
    socklen_t addrlen = sizeof(remaddr);
    //int recvlen;
    int fd;
    //char buf[24];

    if((fd = socket(AF_INET, SOCK_DGRAM, 0))<0){
      std::cout << "cannot create socket" << std::endl;
        return 0;
    }

    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(9002);

    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr))<0){
      std::cout << "bind failed" << std::endl;
        return 0;
    }

    // struct timeval tv;
    // tv.tv_sec = 0;
    // tv.tv_usec = 150000;
    // if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))<0){
    //  perror("Error");
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
        state = stateReceiver->receiveState(FieldTransformationType::None);

        viewerFriend.set_ballposex(state.ball.x);
        viewerFriend.set_ballposey(state.ball.x);
        viewerFriend.set_friend1posex(state.teamYellow[0].x);
        viewerFriend.set_friend1posey(state.teamYellow[0].y);
        viewerFriend.set_friend1yaw(0);//state.teamYellow[0].yaw);
        viewerFriend.set_friend2posex(state.teamYellow[1].x);
        viewerFriend.set_friend2posey(state.teamYellow[1].y);
        viewerFriend.set_friend2yaw(0);//state.teamYellow[1].yaw);
        viewerFriend.set_friend3posex(state.teamYellow[2].x);
        viewerFriend.set_friend3posey(state.teamYellow[2].y);
        viewerFriend.set_friend3yaw(0);//state.teamYellow[2].yaw);

        // how to get team blue??
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

        recvfrom(fd, &bufReceiver, 18, 0, (struct sockaddr *)&remaddr, &addrlen);
        viewerReceiver.ParseFromString(bufReceiver);

        sendVel[0] = viewerReceiver.friend0veldir() - 256;
        sendVel[1] = viewerReceiver.friend1veldir() - 256;
        sendVel[2] = viewerReceiver.friend2veldir() - 256;
        sendVel[3] = viewerReceiver.friend0velesq() - 256;
        sendVel[4] = viewerReceiver.friend1velesq() - 256;
        sendVel[5] = viewerReceiver.friend2velesq() - 256;

        send_commands(sendVel);
        memset(bufReceiver, 0, sizeof(bufReceiver));
        memset(sendVel, 0, sizeof(sendVel));

    }

    return 0;
}

void send_commands(int *vel){
    Command command;

    int aux[6];
    aux[0] = vel[0];
    aux[1] = vel[1];
    aux[2] = vel[2];
    aux[3] = vel[3];
    aux[4] = vel[4];
    aux[5] = vel[5];

    for(int i = 0 ; i < 3 ; i++){
        WheelsCommand wheelsCommand;

        wheelsCommand.leftVel = aux[i+3];
        wheelsCommand.rightVel = aux[i];

        command.commands.push_back(wheelsCommand);
    }

    commandSender->sendCommand(command);
}


void send_debug(){
    vss::Debug debug;

    for(unsigned int i = 0 ; i < 3 ; i++){
        vss::Point point;

        point.x = state.teamYellow[i].x - 10 + rand()%20;
        point.y = state.teamYellow[i].y - 10 + rand()%20;

        debug.stepPoints.push_back(point);
    }

    for(unsigned int i = 0 ; i < 3 ; i++){
        vss::Pose pose;

        pose.x = state.teamYellow[i].x - 10 + rand()%20;
        pose.y = state.teamYellow[i].y - 10 + rand()%20;
        pose.angle = state.teamYellow[i].y - 10 + rand()%20;

        debug.finalPoses.push_back(pose);
    }

    for(unsigned int i = 0 ; i < 3 ; i++){
        vss::Path path;
        vss::Point point_1;
        vss::Point point_2;

        point_1.x = state.teamYellow[i].x;
        point_1.y = state.teamYellow[i].y;

        point_2.x = state.ball.x - 10 + rand()%20;
        point_2.y = state.ball.y - 10 + rand()%20;

        path.points.push_back(point_1);
        path.points.push_back(point_2);
    }

    debugSender->sendDebug(debug);
}
