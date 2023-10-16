#include "organism.h"
#include "window.h"
#include <mutex>



window win(backgroundColor);
std::vector<Organism*> gos;
std::mutex gos_mutex;
std::vector<GameObject*> fos;

std::mutex fos_mutex;

std::vector<GameObject*> copy_fos;

void spawn(int count_kinds, int count_org, bool isGen = false, BornData db = BornData() ){
    std::srand(time(NULL));
    for (int i = 0; i < count_kinds; i++){

        double s, x, y, pr, ph, gR, mS, idP1, idP2, gP1, gP2;
        long long int gen = 0;
        std::vector<std::vector<std::vector<double>>> in_w1 = {}, in_w2 = {};
        std::vector<long long int> forBorn = {};

        if (isGen){
            gen = db.gen;
            pr = int ((db.gen / 0x10000000) / 2.56);
            ph = int (((db.gen - pr) / 0x100000) / 2.56);
            gR = (256 - (db.gen % 0x100));
            mS = 256 - (db.gen % 0x10000 - gR);
            gR = gR / 100;
            s = db.s;
            x = db.x;
            y = db.y;
            in_w1 = db.initial_weights_1;
            in_w2 = db.initial_weights_2;
            forBorn = db.forBornTree;
            forBorn.push_back(0);

        }
        else{
            forBorn = {-1, 0, -2, 0, 0, -3};
             s = std::rand() % 20 + 10;
             x = std::rand() % WorldWidth;
             y = std::rand() % WorldHeight;

             ph = std::rand() % 100;
             pr = 100 - ph;
             gR = (std::rand() % 3 + 1) / 10.;
             mS = std::rand() % 10;
             gen = pr * 0x1000000 + ph * 0x10000 + mS * 0x100 + gR;
        }

        for (int j = 0; j < count_org; j++){
            NeironNet* NeironNetDo = new NeironNet(StandartNeiron1, NeironNet::ActivationFunctionType::Sigmoid, in_w1);
            NeironNet* NeironNetGo = new NeironNet(StandartNeiron2, NeironNet::ActivationFunctionType::Tanh, in_w2);

            int dx = (std::rand() % int(s * 2)) - s;
            int dy = (std::rand() % int(s * 2)) - s;
            Rect r(x + dx, y + dy, s, s);

            Organism* go = new Organism(s, gR, ph, pr, mS, r, &dd, NeironNetDo, NeironNetGo);
            int id = go->getID();
            dd.setIdGen(id, gen);
            forBorn[4] = id;
            forBorn[5] = gen;
            dd.addBornTree(forBorn);

            field.addOrganism(go);
            gos.push_back(go);
            fos.push_back(go);
        }

    }
}


void tickUpdate(){
    while (dd.isPlay){
//        auto point_0 = std::chrono::high_resolution_clock::now();
        std::cout << gos.size() << "\n";
        if (gos.size() == 0){
            dd.isPlay = false;
        }
//        auto point_1 = std::chrono::high_resolution_clock::now();
        {
        std::lock_guard<std::mutex> guard(fos_mutex);
        for (int i = 0; i < gos.size(); i++){
            gos[i]->update();
            if (!gos[i]->IsAlive()){
                int id = gos[i]->getID();
                field.delOrganism(id);
                dd.delIdGen(id);
                gos[i] = nullptr;
                fos[i] = nullptr;

            }
        }
        fos.erase(std::remove(fos.begin(), fos.end(), nullptr), fos.end());
        gos.erase(std::remove(gos.begin(), gos.end(), nullptr), gos.end());
//        auto point_2 = std::chrono::high_resolution_clock::now();

        std::vector<BornData> dbs = dd.getBorns();

        for (BornData db : dbs){
            spawn(1, 1, true, db);
        }
        }
//        auto point_3 = std::chrono::high_resolution_clock::now();
//        auto duration_1 = std::chrono::duration_cast<std::chrono::microseconds>(point_1 - point_0);
//        auto duration_2 = std::chrono::duration_cast<std::chrono::microseconds>(point_2 - point_1);
//        auto duration_3 = std::chrono::duration_cast<std::chrono::microseconds>(point_3 - point_2);
//        auto duration_4 = std::chrono::duration_cast<std::chrono::microseconds>(point_1 - point_0);
//        std::cout << duration_1.count() << " " << duration_2.count() << " " << duration_3.count() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(TPS));
    }
    dd.writeBornTree();

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
        {
            std::lock_guard<std::mutex> guard(fos_mutex);
            win.update(fos);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(FPS));


    }
    win.close();
}



int main(int argv, char** args){



    dd.isPlay = true;
    spawn(100, 10);
    std::thread ticks(tickUpdate);
    frameUpdate();

    ticks.join();
//    tickUpdate();

    return 0;
}


