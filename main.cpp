
#include "organism.h"

window win(color);
std::vector<Organism> gos;
std::vector<GameObject> fos;

void tickUpdate(){
    while (dd.isPlay){
        for (int i = 0; i < gos.size(); i++){
            Organism &go = gos[i];
            go.update();
            if (!go.IsAlive()){
                gos.erase(gos.begin() + i);
            }
        }
        for (BornData db : dd.getBorns()){
            int s = std::rand() % 20;
            Rect r(std::rand() % 900, std::rand() % 500, s, s);
            int pPr = int ((db.gen / 0X10000000) / 2.56), pPh = int (((db.gen - pPr) / 0X100000) / 2.56), gR = 256 - db.gen % 100, mS = 256 - (db.gen % 10000 - gR);

            NeironNet NeironNetDo(StandartNeiron1, NeironNet::ActivationFunctionType::Sigmoid, *db.initial_weights_1);
            NeironNet NeironNetGo(StandartNeiron2, NeironNet::ActivationFunctionType::Tanh, *db.initial_weights_2);
            Organism org(s, gR, pPh, pPr, mS, r, dd, NeironNetDo, NeironNetGo);
            gos.push_back(org);
            fos.push_back(org);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(TPS));
    }

}

void frameUpdate(){

    std::vector<Event> evs;
    while (dd.isPlay){
        evs = win.getEvents();
        for (Event ev : evs){
            if (ev.getType() == 'q'){
                dd.isPlay = false;
            }
        }
        win.update(fos);
        std::this_thread::sleep_for(std::chrono::milliseconds(FPS));


    }
    win.close();
}

int main(int argv, char** args){
    dd.isPlay = true;
    std::srand(time(NULL));

    for (int i = 0; i < 100; i++){
        int s = std::rand() % 20;
        Rect r(std::rand() % 900, std::rand() % 500, s, s);
        int ph = std::rand() % 100, pr = 100 - ph;

        NeironNet NeironNetDo(StandartNeiron1);
        NeironNet NeironNetGo(StandartNeiron2, NeironNet::ActivationFunctionType::Tanh);

        Organism go(s, std::rand() % 30, ph, pr, std::rand() % 50, r, dd, NeironNetDo, NeironNetGo);
        gos.push_back(go);
        fos.push_back(go);
    }

    std::thread ticks(tickUpdate);
    frameUpdate();

    ticks.join();

    return 0;
}


