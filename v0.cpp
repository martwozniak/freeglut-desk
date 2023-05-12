#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
enum
{
 EXIT,
 FULL_WINDOW,
 ASPECT_1_1,
 ORTO,
 FRUST,
 PERSP
};
// ustawienie wartości startowych
GLint skala=FULL_WINDOW;
GLint rzut=ORTO;
GLdouble fovy = 90;
// ustawienie parametrów zakresu rzutni
GLdouble aspect = 1;
GLfloat zakres = 10.0f;
GLfloat blisko = 1.0f;
GLfloat daleko = 10.0f;
static int slices = 16;
static int stacks = 16;

/* Oswietlenie */
// Globalna zmienna do przechowywania stanu oświetlenia
int lightState = 1; // 1 = włączone, 0 = wyłączone
float lightIntensity = 1.0f; // Intensywność światła
/* Oswietlenie */

/* Obrot */
// Globalne zmienne do przechowywania aktualnego kąta obrotu i przesunięcia
float rotation_angle = 0.0f;
float translation_distance = 0.0f;
/* Obrot */


// Funkcja obsługująca oświetlenie
void handleLighting() {
    if (lightState == 1) {
        glEnable(GL_LIGHTING);
        GLfloat light_ambient[] = { lightIntensity, lightIntensity, lightIntensity, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
    } else {
        glDisable(GL_LIGHTING);
    }
}

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

// funkcja do rysowania tekstu
void drawString(float x, float y, float z, const char* string) {
    glRasterPos3f(x, y, z);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void drawCup() {
    float cupRadius = 0.05f;
    float cupHeight = 0.1f;

    // Rysuj kubek
    glColor3f(0.0f, 0.0f, 1.0f);  // kolor niebieski
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, cupRadius, cupRadius, cupHeight, 20, 20);

    // Napis na kubku
    glColor3f(1.0f, 1.0f, 1.0f);  // kolor biały
    drawString(-0.04f, cupHeight/2, cupRadius + 0.01f, "Politechnika Czestochowska");
    drawString(-1.08f, cupHeight/4, cupRadius + 1.05f, "Marcin Wozniak");
}


void drawDesk() {
    float deskTopThickness = 0.05f;
    float deskLegThickness = 0.05f;
    float deskWidth = 0.5f;
    float deskLength = 1.0f;
    float deskHeight = 0.5f;

    glPushMatrix();
    glColor3f(0.65f, 0.35f, 0.25f);  // kolor brązowy
    // Rysuj górną część biurka
    glPushMatrix();
    glTranslatef(0.0f, deskHeight, 0.0f);
    glScalef(deskLength, deskTopThickness, deskWidth);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Rysuj nogi biurka
    glPushMatrix();
    glTranslatef(deskLength/2 - deskLegThickness/2, deskHeight/2, deskWidth/2 - deskLegThickness/2);
    glScalef(deskLegThickness, deskHeight, deskLegThickness);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-deskLength/2 + deskLegThickness/2, deskHeight/2, deskWidth/2 - deskLegThickness/2);
    glScalef(deskLegThickness, deskHeight, deskLegThickness);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(deskLength/2 - deskLegThickness/2, deskHeight/2, -deskWidth/2 + deskLegThickness/2);
    glScalef(deskLegThickness, deskHeight, deskLegThickness);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-deskLength/2 + deskLegThickness/2, deskHeight/2, -deskWidth/2 + deskLegThickness/2);
    glScalef(deskLegThickness, deskHeight, deskLegThickness);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

void drawPlant() {
    float plantHeight = 0.1f;
    float plantRadius = 0.025f;
    float potHeight = 0.05f;
    float potRadius = 0.035f;

    // Rysuj liściastą część rośliny
    glColor3f(0.0f, 1.0f, 0.0f);  // kolor zielony
    glPushMatrix();
    glTranslatef(0.0f, potHeight, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(plantRadius, plantHeight, 20, 20);
    glPopMatrix();

    // Rysuj doniczkę
    glColor3f(1.0f, 1.0f, 0.0f);  // kolor żółty
    glPushMatrix();
    GLUquadric* quad = gluNewQuadric();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, potRadius, potRadius, potHeight, 20, 20);
    glPopMatrix();
}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);
    // Zmiana kierunku kamery
    glLoadIdentity();
    gluLookAt(2.0, 1.0, 2.0,  // Pozycja kamery
              0.0, 0.0, 0.0,  // Punkt, na który kamera patrzy
              0.0, 1.0, 0.0); // Kierunek 'góry'
    // Zastosuj przesunięcie i obrót do wszystkich obiektów
    glRotatef(rotation_angle, 0.0f, 1.0f, 0.0f);
    glTranslatef(translation_distance, 0.0f, 0.0f);


    glPushMatrix();
    // Przesunięcie wszystkich obiektów dalej od kamery
    //glTranslatef(0.0f, 0.0f, -1.0f);

    drawDesk();
    // Dodaj roślinę na biurko
    glPushMatrix();
    glTranslatef(0.3f, 0.5f, 0.0f);  // przesuń na blat biurka
    drawPlant();
    glPopMatrix();

    // Dodaj kubek na biurko
    glPushMatrix();
    glTranslatef(-0.3f, 0.55f, 0.0f);  // przesuń na blat biurka
    drawCup();
    glPopMatrix();



    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
       case 'l': // Włącz / wyłącz oświetlenie
            lightState = !lightState;
            handleLighting();
            break;
        case '[': // Zwiększ intensywność światła
            lightIntensity += 0.1f;
            if(lightIntensity > 1.0f) lightIntensity = 1.0f;
            handleLighting();
            break;
        case ']': // Zmniejsz intensywność światła
            lightIntensity -= 0.1f;
            if(lightIntensity < 0.0f) lightIntensity = 0.0f;
            handleLighting();
            break;
        case 'a': // obrót w lewo
            rotation_angle -= 5.0f;
            break;
        case 'd': // obrót w prawo
            rotation_angle += 5.0f;
            break;
        case 'w': // przesunięcie do przodu
            translation_distance += 0.1f;
            break;
        case 's': // przesunięcie do tyłu
            translation_distance -= 0.1f;
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };
/* Menu */
void Menu(int value) {

    switch (value) {
        case FULL_WINDOW:
            //resize(1280,720);
            break;
        case ASPECT_1_1:
            // Kod do obsługi skalowania - skala 1:1
            break;
        case ORTO:
            break;
        case FRUST:
            // Kod do obsługi rzutowania frustum
            break;
        case PERSP:
            // Kod do obsługi rzutowania perspective
            break;
        case EXIT:
            exit(0);
    }
    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Projekt zaliczeniowy - Marcin Wozniak");

    // Menu

    glutCreateMenu(Menu); // utworzenie menu podręcznego
    glutAddMenuEntry("Rodzaj skalowania - całe okno", FULL_WINDOW);
    glutAddMenuEntry("Rodzaj skalowania - skala 1:1", ASPECT_1_1);
    glutAddMenuEntry("Rzutowanie ortogonalne", ORTO);
    glutAddMenuEntry("Rzutowanie frustum", FRUST);
    glutAddMenuEntry("Rzutowanie perspective", PERSP);
    glutAddMenuEntry("Wyjście", EXIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON); // określenie przycisku myszki obsługującej menu podręczne


    // Koniec Menu

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}
