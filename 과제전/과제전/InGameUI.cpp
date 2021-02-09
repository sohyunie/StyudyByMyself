#include "Standard.h"
#include "InGameUI.h"
#include "InGameManager.h"

InGameUI::InGameUI() {

}

void InGameUI::PrintInGameUI(GLuint s_program) {
        if (this->isPlayGame == true) {
            glClearColor(0.0, 0.0, 0.0, 0.0);
            string text = "TIME : " + to_string(InGameManager::GetInstance().currentTime()).substr(0, 4);
            const char* string = text.data();
            glColor3f(1.0f, 1.0f, 1.0f);
            glRasterPos2f(-0.9, 0.9);  // ���� ����� ��ġ ����

            int len = (int)strlen(string);
            for (int i = 0; i < len; i++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
            }

             text = "Bead : " + to_string(InGameManager::GetInstance().GetBeadNumber());
             string = text.data();
             glColor3f(0.5f, 0.5f, 0.5f);
             glRasterPos2f(-0.9, 0.8);  // ���� ����� ��ġ ����

             len = (int)strlen(string);
             for (int i = 0; i < len; i++) {
                 glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
             }
        }
        //else {
        //    string text = "SPACE BAR�� ������ ����� �����մϴ�.";
        //    const char* string = text.data();
        //    glColor3f(0.5f, 0.5f, 0.5f);
        //    glRasterPos2f(-0.2, -0.2);  // ���� ����� ��ġ ����

        //    int len = (int)strlen(string);
        //    for (int i = 0; i < len; i++) {
        //        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
        //    }
        //    if (isClear) {
        //        text = "GAME CLEAR";
        //        const char* string = text.data();
        //        glColor3f(0.5f, 0.5f, 0.5f);
        //        glRasterPos2f(-0.2, 0.0);  // ���� ����� ��ġ ����

        //        int len = (int)strlen(string);
        //        for (int i = 0; i < len; i++) {
        //            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
        //        }
        //    }
        //    else {
        //        text = "GAME OVER";
        //        const char* string = text.data();
        //        glColor3f(0.5f, 0.5f, 0.5f);
        //        glRasterPos2f(-0.2, 0.0);  // ���� ����� ��ġ ����

        //        int len = (int)strlen(string);
        //        for (int i = 0; i < len; i++) {
        //            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
        //        }
        //    }
        //}
}