#include "Application.h"

int main() {
    Application app;
    app.Init();
    app.Logic();
    app.Release();
}