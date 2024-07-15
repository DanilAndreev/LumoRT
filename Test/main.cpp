#include "Application.h"
#include "ApplicationRT.h"

int main() {
    ApplicationRT app;
    app.Init();
    app.Logic();
    app.Release();
}