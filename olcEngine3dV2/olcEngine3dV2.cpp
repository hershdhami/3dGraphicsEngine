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

struct mat4x4 {
    //Row followed by column
    float m[4][4] = { 0 };
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
        mat4x4 matProj;
        float fTheta;

        void MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m) {
            o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
            o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
            o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
            float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

            if (w != 0.0f) {
                o.x /= w; 
                o.y /= w; 
                o.z /= w;
            }
        }

    public:
        bool OnUserCreate() override {
            meshCube.tris = {
                //NOTE: Y is really Z and Z is really Y in this defined coordinate system

                //SOUTH OF CUBE
                {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},

                //EAST OF CUBE
                {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
                {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
                
                //NORTH OF CUBE
                {1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
                {1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},

                //WEST OF CUBE
                {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},

                //TOP
                {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
                {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},

                //BOTTOM
                {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
                {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
            };

            //Projection Matrix
            float fNear = 0.1f;
            float fFar = 1000.0f;
            float fFov = 90.0f;
            float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
            float fFovRad = 1.0f / tanf(fFov * 0.5f / (180.0f * 3.14159f));

            matProj.m[0][0] = fAspectRatio * fFovRad;
            matProj.m[1][1] = fFovRad;
            matProj.m[2][2] = fFar / (fFar - fNear);
            matProj.m[3][2] = -((fFar * fNear) / (fFar - fNear));
            matProj.m[2][3] = 1.0f;
            matProj.m[3][3] = 0.0f;

            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override {
            Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

            mat4x4 matRotZ, matRotX;
            fTheta += 1.0 * fElapsedTime;

            // Rotation Z
            matRotZ.m[0][0] = cosf(fTheta);
            matRotZ.m[0][1] = sinf(fTheta);
            matRotZ.m[1][0] = -sinf(fTheta);
            matRotZ.m[1][1] = cosf(fTheta);
            matRotZ.m[2][2] = 1;
            matRotZ.m[3][3] = 1;

            // Rotation X
            matRotX.m[0][0] = 1;
            matRotX.m[1][1] = cosf(fTheta * 0.5f);
            matRotX.m[1][2] = sinf(fTheta * 0.5f);
            matRotX.m[2][1] = -sinf(fTheta * 0.5f);
            matRotX.m[2][2] = cosf(fTheta * 0.5f);
            matRotX.m[3][3] = 1;

            //Draw Triangles
            //Because triangles are inside a vector we can use auto?
            //Need to condense 3D space to the 2d screen
            //Note: This is all dependant on the field of view
                //Assume the field of view is Theta

            for (auto tri : meshCube.tris) {
                triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

                MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
                MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
                MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

                MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
                MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
                MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

                triTranslated = triRotatedZX;
                triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
                triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
                triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

                MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
                MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
                MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);


                // Scale into view
                
                triProjected.p[0].x += 10.0f; triProjected.p[0].y += 10.0f;
                triProjected.p[1].x += 10.0f; triProjected.p[1].y += 10.0f;
                triProjected.p[2].x += 10.0f; triProjected.p[2].y += 10.0f;

                triProjected.p[0].x *= 0.05f * (float)ScreenWidth();
                triProjected.p[0].y *= 0.05f * (float)ScreenHeight();
                triProjected.p[1].x *= 0.05f * (float)ScreenWidth();
                triProjected.p[1].y *= 0.05f * (float)ScreenHeight();
                triProjected.p[2].x *= 0.05f * (float)ScreenWidth();
                triProjected.p[2].y *= 0.05f * (float)ScreenHeight();
                


                DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
                    triProjected.p[1].x, triProjected.p[1].y,
                    triProjected.p[2].x, triProjected.p[2].y,
                    PIXEL_SOLID, FG_WHITE);

            }

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