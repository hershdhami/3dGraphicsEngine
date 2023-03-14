#include "olcConsoleGameEngine.h"
using namespace std;

struct vec3d {
    float x, y, z;
};

struct triangle {
    vec3d p[3];
};

struct mesh {
    vector<triangle> tris;
};

class olcEngine3d : public olcConsoleGameEngine {
    public:
        olcEngine3d() {
            m_sAppName = L"3D Demo";
        }
        //Almost always use wstring on Windows
        //wchar is 2 bytes long

    private:
        mesh meshCube;

    public:
        bool OnUserCreate() override {
            meshCube.tris = {
                //SOUTH OF CUBE
                {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},

                //EAST OF CUBE
                {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
                {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
                
                //Add other directions
            };

            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override {
            return true;
        }
};

int main()
{
    olcEngine3d demo;
    if (demo.ConstructConsole(256,240,4,4)) {
        demo.Start();
    }
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

/*
* PUBLIC VS PRIVATE VS PROTECTED INHERITANCE
* 
class Base {
  public:
    int x;
  protected:
    int y;
  private:
    int z;
};

class PublicDerived: public Base {
  // x is public
  // y is protected
  // z is not accessible from PublicDerived
};
class ProtectedDerived: protected Base {
  // x is protected
  // y is protected
  // z is not accessible from ProtectedDerived
};

class PrivateDerived: private Base {
  // x is private
  // y is private
  // z is not accessible from PrivateDerived
};
*/