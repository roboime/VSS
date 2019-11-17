#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>
#include <Communications/DebugSender.h>
#include "cstdlib"
#include "roboime.pb.h"

using namespace vss;

IStateReceiver *stateReceiver;
ICommandSender *commandSender;
IDebugSender *debugSender;

State state;

void send_commands();
void send_debug();

int main(int argc, char** argv){
    srand(time(NULL));

    stateReceiver = new StateReceiver();
    commandSender = new CommandSender();
    debugSender = new DebugSender();

    stateReceiver->createSocket();
    commandSender->createSocket(TeamType::Yellow);
    debugSender->createSocket(TeamType::Yellow);

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    RoboimeData::DataBall dataBall;
    RoboimeData::DataYellow dataYellow;
    RoboimeData::DataBlue dataBlue;
    RoboimeData::DataReceiver dataReceiver;

    while(true){
        state = stateReceiver->receiveState(FieldTransformationType::None);
        
        dataBall.set_ballposex(state.ball.x);
        dataBall.set_ballposey(state.ball.y);
        dataBall.set_ballspeedx(state.ball.speedX);
        dataBall.set_ballspeedy(state.ball.speedY);
        
        dataYellow.set_yellow0posex(state.teamYellow[0].x);
        dataYellow.set_yellow0posey(state.teamYellow[0].y);
        dataYellow.set_yellow0angle(state.teamYellow[0].angle);
        dataYellow.set_yellow0speedx(state.teamYellow[0].speedX);
        dataYellow.set_yellow0speedy(state.teamYellow[0].speedY);
        dataYellow.set_yellow0speedangle(state.teamYellow[0].speedAngle);
        dataYellow.set_yellow1posex(state.teamYellow[1].x);
        dataYellow.set_yellow1posey(state.teamYellow[1].y);
        dataYellow.set_yellow1angle(state.teamYellow[1].angle);
        dataYellow.set_yellow1speedx(state.teamYellow[1].speedX);
        dataYellow.set_yellow1speedy(state.teamYellow[1].speedY);
        dataYellow.set_yellow1speedangle(state.teamYellow[1].speedAngle);
        dataYellow.set_yellow2posex(state.teamYellow[2].x);
        dataYellow.set_yellow2posey(state.teamYellow[2].y);
        dataYellow.set_yellow2angle(state.teamYellow[2].angle);
        dataYellow.set_yellow2speedx(state.teamYellow[2].speedX);
        dataYellow.set_yellow2speedy(state.teamYellow[2].speedY);
        dataYellow.set_yellow2speedangle(state.teamYellow[2].speedAngle);

        dataBlue.set_blue0posex(state.teamBlue[0].x);
        dataBlue.set_blue0posey(state.teamBlue[0].y);
        dataBlue.set_blue0angle(state.teamBlue[0].angle);
        dataBlue.set_blue0speedx(state.teamBlue[0].speedX);
        dataBlue.set_blue0speedy(state.teamBlue[0].speedY);
        dataBlue.set_blue0speedangle(state.teamBlue[0].speedAngle);
        dataBlue.set_blue1posex(state.teamBlue[1].x);
        dataBlue.set_blue1posey(state.teamBlue[1].y);
        dataBlue.set_blue1angle(state.teamBlue[1].angle);
        dataBlue.set_blue1speedx(state.teamBlue[1].speedX);
        dataBlue.set_blue1speedy(state.teamBlue[1].speedY);
        dataBlue.set_blue1speedangle(state.teamBlue[1].speedAngle);
        dataBlue.set_blue2posex(state.teamBlue[2].x);
        dataBlue.set_blue2posey(state.teamBlue[2].y);
        dataBlue.set_blue2angle(state.teamBlue[2].angle);
        dataBlue.set_blue2speedx(state.teamBlue[2].speedX);
        dataBlue.set_blue2speedy(state.teamBlue[2].speedY);
        dataBlue.set_blue2speedangle(state.teamBlue[2].speedAngle);
        
        //std::cout << state << std::endl;
	    //std::cout << state.ball << std::endl;
	    //std::cout << state.teamYellow[1].speedX << std::endl;
        std::cout << "ta funcionando =)" << dataYellow.yellow1posex() << std::endl;
        send_commands();
        send_debug();
    }

    return 0;
}

void send_commands(){
    Command command;

    for(int i = 0 ; i < 3 ; i++){
        command.commands.push_back(WheelsCommand(i*40, i*30));
    }

    commandSender->sendCommand(command);
}


void send_debug(){
    vss::Debug debug;

    for(unsigned int i = 0 ; i < 3 ; i++){
        debug.stepPoints.push_back(Point(85 + rand()%20, 65 + rand()%20));
    }

    for(unsigned int i = 0 ; i < 3 ; i++){
        debug.finalPoses.push_back(Pose(85 + rand()%20, 65 + rand()%20, rand()%20));
    }

    for(unsigned int i = 0 ; i < 3 ; i++){
        vss::Path path;
        path.points.push_back(Point(85, 65));
        path.points.push_back(Point(85 + rand()%20, 65 + rand()%20));
    }

    debugSender->sendDebug(debug);
}
