#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <unistd.h>

const int num_filosofos = 5;
int fideos = 256;
std::vector<int> estomagos(num_filosofos, 0);
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

class Filosofo {
public:
    int id;

    Filosofo(int id) : id(id) {}

    void pensar() {
        int pensar_tiempo = generar_tiempo_aleatorio(1, 5);
        std::cout << "Filosofo " << id << " está pensando (" << pensar_tiempo << "s);" << std::endl;
        sleep(pensar_tiempo);
    }

    void comer() {
        pthread_mutex_lock(&mtx);

        if (fideos > 0) {
            int porcion = std::min(generar_cantidad_aleatoria(1, 50), fideos);
            fideos -= porcion;
            estomagos[id] += porcion;

            std::cout << "Filosofo " << id << " está comiendo" << std::endl;
            std::cout << "Filosofo " << id << " incrementó el valor del estómago (" << estomagos[id] - porcion << " + " << porcion << " = " << estomagos[id] << ")" << std::endl;
        }

        pthread_mutex_unlock(&mtx);
    }

    static void* filosofo_thread(void* arg) {
        Filosofo* filosofo = static_cast<Filosofo*>(arg);

        while (fideos > 0) {
            filosofo->pensar();
            filosofo->comer();
        }

        return nullptr;
    }

    int generar_tiempo_aleatorio(int min, int max) {
        return rand() % (max - min + 1) + min;
    }

    int generar_cantidad_aleatoria(int min, int max) {
        return rand() % (max - min + 1) + min;
    }
};

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    pthread_t threads[num_filosofos];

    std::cout << "Fideos = " << fideos << std::endl;

    std::vector<Filosofo> filosofos;
    for (int i = 0; i < num_filosofos; ++i) {
        filosofos.emplace_back(i);
    }

    for (int i = 0; i < num_filosofos; ++i) {
        pthread_create(&threads[i], nullptr, &Filosofo::filosofo_thread, &filosofos[i]);
    }

    for (int i = 0; i < num_filosofos; ++i) {
        pthread_join(threads[i], nullptr);
    }

    for (int i = 0; i < num_filosofos; ++i) {
        std::cout << "Estómago del filósofo " << i << " = " << estomagos[i] << std::endl;
    }

    return 0;
}
