#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <geode.globed/api.hpp>

using namespace geode::prelude;

// Перехватываем игровой уровень Geometry Dash 2.2
class $modify(FNFPlayLayer, PlayLayer) {
    
    // Функция отслеживает нажатия кнопок
    void pushButton(PlayerButton button, bool isPlayer2) {
        PlayLayer::pushButton(button, isPlayer2);
        
        // Мапим кнопки под стрелки FNF (0 = Влево, 1 = Вниз, 2 = Вверх, 3 = Вправо)
        int arrowIndex = -1;
        if (button == PlayerButton::Left) arrowIndex = 0;
        if (button == PlayerButton::Jump) arrowIndex = 2; // Прыжок назначен на стрелку Вверх
        
        if (arrowIndex != -1) {
            // Отправляем пакет с нажатой стрелкой в сеть через Globed
            std::string networkPacket = "fnf_press_" + std::to_string(arrowIndex);
            GlobedAPI::sendDataToAll(networkPacket);
        }
    }
};

// Функция-приемник сетевых данных от второго игрока
class FNFNetworkHandler {
public:
    void onReceiveNetworkData(std::string message) {
        // Проверяем, что пакет прилетел от нашего мода
        if (message.rfind("fnf_press_", 0) == 0) {
            // Извлекаем индекс стрелки, которую нажал второй игрок
            int opponentArrow = std::stoi(message.substr(10));
            
            // Здесь будет срабатывать анимация для иконки оппонента
            log::info("Оппонент нажал стрелку: {}", opponentArrow);
        }
    }
};
