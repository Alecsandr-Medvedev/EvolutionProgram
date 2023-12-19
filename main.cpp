#include "organism.h"
#include "window.h"
#include <mutex>



window win(backgroundColor);
std::vector<Organism*> gos;
std::mutex gos_mutex;
std::vector<GameObject*> fos;
//std::vector<GameObject*> f = {};

std::mutex fos_mutex;

std::vector<GameObject*> copy_fos;
 int orgs_count = 0;

void spawn(int count_kinds, int count_org, bool isGen = false, BornData db = BornData() ){
    std::srand(time(NULL));
    for (int i = 0; i < count_kinds; i++){

        double s, x, y, pr, ph, gR, mS, idP1, idP2, gP1, gP2;
        long long int gen = 0;
        Genom genom;
        std::vector<std::vector<std::vector<double>>> in_w1 = {}, in_w2 = {};
        std::vector<long long int> forBorn = {};

        if (isGen){
            genom = db.gen;
            gen = db.gen.convertToColor();
            pr = db.gen.pointPr;
            ph = db.gen.pointPh;
            gR = db.gen.growthRate;
            mS = db.gen.maxSpeed;

            s = db.s;
            x = db.x;
            y = db.y;
            in_w1 = db.gen.initial_weights_1;
            in_w2 = db.gen.initial_weights_2;
            forBorn = db.forBornTree;

            forBorn.push_back(0);
            forBorn.push_back(0);

        }
        else{
            forBorn = {-1, 0, -2, 0, 0, -3};
            s = std::rand() % 20 + 10;
            x = std::rand() % WorldWidth;
            y = std::rand() % WorldHeight;

            ph = std::rand() % 256;
            pr = 255 - ph;
            gR = (std::rand() % 255 + 1) / 100.;
            mS = (std::rand() % 256) / 100.;

            genom.pointPh = ph;
            genom.pointPr = pr;
            genom.growthRate = gR;
            genom.maxSpeed = mS;

            gen = genom.convertToColor();

        }
        for (int j = 0; j < count_org; j++){
            NeironNet* NeironNetDo = new NeironNet(StandartNeiron1, NeironNet::ActivationFunctionType::Sigmoid, in_w1);
            NeironNet* NeironNetGo = new NeironNet(StandartNeiron2, NeironNet::ActivationFunctionType::Tanh, in_w2);

            int dx = (std::rand() % int(s * 2)) - s;
            int dy = (std::rand() % int(s * 2)) - s;
            Rect r(x + dx, y + dy, s, s);

            Organism* go = new Organism(s, gR, ph, pr, mS, r, &dd, NeironNetDo, NeironNetGo);
            int id = go->getID();
            dd.setIdGen(id, genom);
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
    {
        std::lock_guard<std::mutex> guard(fos_mutex);
//        std::cout << "\n" << gos.size() << "\n";
//    auto start_time = std::chrono::steady_clock::now();



        orgs_count = gos.size();
        if (orgs_count == 0){
            dd.isPlay = false;
        }


        int count_die = 0;
        for (int i = 0; i < orgs_count; i++){
            gos[i]->update();
            if (!gos[i]->IsAlive()){
                    count_die++;
                    int id = gos[i]->getID();
                    field.delOrganism(id);
                    dd.delIdGen(id);
                    gos[i] = nullptr;
                    fos[i] = nullptr;
            }
        }
//        std::cout << count_die << "\t";
        {
        fos.erase(std::remove(fos.begin(), fos.end(), nullptr), fos.end());
        gos.erase(std::remove(gos.begin(), gos.end(), nullptr), gos.end());
        }

        std::vector<BornData> dbs = dd.getBorns();
        int count_born = 0;
        for (BornData db : dbs){
            count_born++;
            spawn(1, 1, true, db);

        }
//        std::cout << count_born << "\t";
    }
        std::this_thread::sleep_for(std::chrono::milliseconds(TPS));
//        auto end_time = std::chrono::steady_clock::now();
//        auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
//
//        std::cout << elapsed_ns.count() << " ns\n";
        year++;
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
            win.startDraw();
            win.draw(fos);
            win.draw(field.getZonesObjects());
            win.update();

        }
        std::this_thread::sleep_for(std::chrono::milliseconds(FPS));


    }
    win.close();
}

void createZones(int count_zones){
    for (int i = 0; i < count_zones; i++){
        int x = getRandNum(1, WorldWidth - 1, 1);
        int y = getRandNum(1, WorldHeight - 1, 1);
        int t = getRandNum(0, 100, 1), v = getRandNum(0, 100, 1), il = getRandNum(0, 100, 1);
        Rect r(x, y, getRandNum(x, WorldWidth , 1), getRandNum(y, WorldHeight, 1));
        zone z(t, v, il, r, &dd);
        field.addZone(&z);
    }
}


int main(int argv, char** args){

//    dd.isPlay = true;
//    spawn(80, 5);
//    createZones(15);
//    std::thread ticks(tickUpdate);
//    frameUpdate();
//
//    ticks.join();

    std::vector<NeironNet> neirons;

    for (int i = 0; i < 2000; i++){
        neirons.push_back(NeironNet(StandartNeiron1));
    }

    std::vector<double> times;
    for (int i = 0; i < 100; i++){
        std::vector<double> l = {};
        for (int i = 0; i < 7; i++){
            l.push_back(getRandNum(1, 10, 1));
        }
        auto ts = std::chrono::steady_clock::now();
        for (NeironNet neir : neirons){
            neir.forward(l);
        }
        auto te = std::chrono::steady_clock::now();
        auto elapsed_ns = std::chrono::duration_cast<std::chrono::milliseconds>(te - ts);
        times.push_back(elapsed_ns.count());
    }
    double sum = 0;
    for (int i = 0; i < 100; i++){
        sum += times[i];
    }
    std::cout << sum / 100;
// 55836000 ns
// 427171000 ns

    return 0;
}


