#include <Phoenix/Core/App.h>

class Game: public Phoenix::App {
public:

    void Initialize() override;
    void Update(float deltaTime) override;
    void Render() override;
    void Shutdown() override;


};
