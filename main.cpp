/*!
    /file main.cpp
    /brief main file which utilises all components and libraries for rendering a Flipbook using Opengl
*/

#include "main.h"

/*!
    \fn setHexColor(int x)
    \brief set given hex color as rgb value.
    setHexColor receives an Integer parameter as Hexadecimal.
    Sets the hex color received as argument.
*/
void setHexColor(int color /*!< color received as Hexadecimal*/){
    GLfloat b = (color%0xff)/255.0;
    GLfloat g = ((color>>8)%0xff)/255.0;
    GLfloat r = ((color>>16)%0xff)/255.0;
    glColor3f(r, g, b); /*!< Detailed description after the member */
}

/*!
    \fn selectFlippingPageType()
    \brief Select Page Flip Type.
    Selects the type of page flip chosen by the user.
    flipId -> holds the page flipid chosen by the user.
        which is set by the user from input interaction.

*/
void selectFlippingPageType(){
    if(flipId == 0){
        pageFlipLimit = 270;
        glRotatef(pageAngle,0.0,-1.0,0.0); /*!< Book Flip type */
    }
    else if(flipId == 1){
        pageFlipLimit = 320;
        glRotatef(pageAngle,-1.0,-1.0,0.0); /*!< Writing Page Flip */
    }
    else if(flipId == 2){
        pageFlipLimit = 350;
        glRotatef(pageAngle,-1.0,-0.0,0.0); /*!< Notepad Page Flip */
    }
    else if(flipId == 3){
        pageFlipLimit = 340;
        glRotatef(pageAngle,-1.0,0.0,-1.0); /*!< Special Page Flip */
    }
}

/*!
    \fn selectViewingAngle()
    \brief Select Viewing Type
    Selects the view type based on user input interaction
        Selection based on viewId value.
*/
void selectViewingAngle(){
    if(viewId == 0){
        gluLookAt(0,0,1900,0,0,0,0,50,0); /*!< Normal View */
    }
    else if(viewId == 1){
        gluLookAt(0,700,2500,0,0,0,0,10,0); /*!< Top View */
    }
    else if(viewId == 2){
        gluLookAt(0,-700,2500,20,0,-50,0,50,0); /*!< Bottom View */
    }
    else if(viewId == 3){
        viewAngleLimit = 720;
        gluLookAt(-500,0,3000,0,0,0,0,80,0); /*!< Horizontal View */
        glRotatef(viewAngle,0.0,-1.0,-1.0); /*!< 360 deg rotation */
    }
    else if(viewId == 4){
        viewAngleLimit = 720;
        gluLookAt(0,-500,3000,0,0,0,0,80,0); /*!< Vertical View */
        glRotatef(viewAngle,-1.0,0.0,0.0); /*!< 360 deg rotation */
    }
    else if(viewId == 5){
        viewAngleLimit = 720;
        gluLookAt(0,500,3000,0,0,0,0,80,0); /*!< Horizontal and Vertical View */
        glRotatef(viewAngle,1.0,-1.0,1.0); /*!< 360 deg rotation */
    }
}

/*!
    \fn flipPage()
    \brief Increase the angle of rotation
    flipPage increases the angle for rotation until the limit value set on idle execution.
    pageAngle -> for page rotation until pageLimit
    viewAngle -> for changing the viewing angle until viewAngleLimit
*/
void flipPage(){
    if(startFlipping == 1 && pauseFlipping == 0){
        if(pageAngle <= pageFlipLimit){
            pageAngle += speed; /*!< Increase pageAngle based on the chosen speed */
        }
        else{
            pageAngle -= pageFlipLimit; /*!< Start from 0 after limit is crossed */
            pageId = (pageId + 1)%noOfPages; /*!< Start from page 1 after n flips */
        }
    }
    if(viewId >= 3){
        if(viewAngle <= viewAngleLimit)
            viewAngle += speed; /*!< Increase viewAngle based on chosen speed */
        else
            viewAngle -= viewAngleLimit; /*!< Start from 0 after limit is crossed */
    }
}

/*!
    \fn idleStateExecution()
    \brief Called on idle state of Opengl execution.
    idleStateExecution is called always on idle state.
    renders flipPage for controlling the angle for pageflip and viewing.
*/
void idleStateExecute(){
    flipPage();
    glutPostRedisplay();
}

/*!
    \fn initReshape(int w, int h)
    \brief Callback for window reshape
    Called initially during rendering and on window reshape.
    sets the PROJECTION_MATRIX and based on screen display
    Welcome screen -> Parallel Projection
    Main screen -> Perspective Projection
*/
void initReshape(int w, int h){
    window_height = h;
    window_width = w;
    glViewport(0, 0, w, h);
    glClearColor(0.0,0.0,0.0,1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(showWelcomeScreen == 1) /*!< Welcome Screen -> Parallel Projection */
    {
        if(w<=h)
            glOrtho(-WINDOW_WIDTH,WINDOW_WIDTH,-WINDOW_HEIGHT*(GLfloat)h/(GLfloat)w,WINDOW_HEIGHT*(GLfloat)h/(GLfloat)w,-MAX_NO_PAGES*BOOK_THICKNESS,MAX_NO_PAGES*BOOK_THICKNESS);
        else
            glOrtho(-WINDOW_WIDTH*(GLfloat)w/(GLfloat)h,WINDOW_WIDTH*(GLfloat)w/(GLfloat)h,-WINDOW_HEIGHT,WINDOW_HEIGHT,-MAX_NO_PAGES*BOOK_THICKNESS,MAX_NO_PAGES*BOOK_THICKNESS);
    }
    else
        gluPerspective(45,(w/h),20,20000); /** Main Page -> Perspective Projection */
    glMatrixMode(GL_MODELVIEW);
}

/*!
    \fn display()
    \brief Render function used in Opengl
    the Render function used to display
    showWelcomeScreen decides welcome screen or main screen to be rendered.
    Flipbook created, room setup, added pages for the flipbook,
    transformations for flipping the pages, lighting and shading operations.
*/
void display(){
    char string1[]="\n The \n   TETRIS  \n Game";
    char string[]="\n\n  Game Over!";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    if(showWelcomeScreen == 1){ /*!< Welcome Screen */
        initReshape(window_width, window_height);
        runWelcomeScreen();
    }
    else{ /*!< Main Screen */
        GLfloat LIGHT_AMBIENT[]={0.5f,0.59f, 0.57f, 0.0f};
        GLfloat LIGHT_DIFFUSE[]={0.73f,0.74f,0.76f,0.22f};
        GLfloat LIGHT_POSITION[]={350.0f, 300.0f, 20.0f, 999.0f};
        GLfloat mat_ambient[]={0.7f,0.7f,0.7f,1.0f};
        GLfloat mat_diffuse[]={0.5f,0.5f,0.5f,1.0f};
        GLfloat mat_specular[]={1.0f,1.0f,1.0f,1.0f};
        GLfloat mat_shininess[]={50.0f};
        initReshape(window_width, window_height);
        /*!< Lighting and Shading */
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glLightfv(GL_LIGHT1, GL_AMBIENT, LIGHT_AMBIENT);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, LIGHT_DIFFUSE);
        glLightfv(GL_LIGHT1, GL_POSITION, LIGHT_POSITION);
        selectViewingAngle();
        drawRoom();
        glPushMatrix();
            glTranslatef(-100,-100,0);
            Book *flipbook=new Book(); /*!< Flipbook Creation */
            glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
            glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
            glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
            glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
            /*!< Shading */
            flipbook->addPage(PAGE_TYPE_TEXT,string1);
            flipbook->addPage(PAGE_TYPE_DRAWING,level);
            flipbook->addPage(PAGE_TYPE_DRAWING,level1);
            flipbook->addPage(PAGE_TYPE_DRAWING,level2);
            flipbook->addPage(PAGE_TYPE_DRAWING,level3);
            flipbook->addPage(PAGE_TYPE_DRAWING,level4);
            flipbook->addPage(PAGE_TYPE_DRAWING,level5);
            flipbook->addPage(PAGE_TYPE_DRAWING,level6);
            flipbook->addPage(PAGE_TYPE_DRAWING,level7);
            flipbook->addPage(PAGE_TYPE_DRAWING,level8);
            flipbook->addPage(PAGE_TYPE_DRAWING,level9);
            flipbook->addPage(PAGE_TYPE_DRAWING,level10);
            flipbook->addPage(PAGE_TYPE_DRAWING,level11);
            flipbook->addPage(PAGE_TYPE_DRAWING,level12);
            flipbook->addPage(PAGE_TYPE_DRAWING,level13);
            flipbook->addPage(PAGE_TYPE_DRAWING,level14);
            flipbook->addPage(PAGE_TYPE_DRAWING,level15);
            flipbook->addPage(PAGE_TYPE_DRAWING,level16);
            flipbook->addPage(PAGE_TYPE_DRAWING,level17);
            flipbook->addPage(PAGE_TYPE_DRAWING,level18);
            flipbook->addPage(PAGE_TYPE_DRAWING,level19);
            flipbook->addPage(PAGE_TYPE_DRAWING,level20);
            flipbook->addPage(PAGE_TYPE_DRAWING,level21);
            flipbook->addPage(PAGE_TYPE_DRAWING,level22);
            flipbook->addPage(PAGE_TYPE_TEXT,string);
            glColor3f(0,0,0);
            if(startFlipping == 0){ /*!< Book laying down on the table */
                glTranslatef(-80, -110, -150);
                glRotatef(-90, 1, 0, 0);
            }
            else{
                glTranslatef(translateBookbyX,translateBookbyY,50);
            }
            flipbook->renderBook(); /*!< Render Flipbook */
            glPushMatrix();
                glTranslatef(0,PAGE_HEIGHT+BOOK_BORDER_SIZE,0);
                selectFlippingPageType(); /*!< Flip current Page */
                glTranslatef(0,-PAGE_HEIGHT-BOOK_BORDER_SIZE,0);
                flipbook->renderPage(pageId);
                glFlush();
            glPopMatrix();
            flipbook->renderPage(pageId+1); /*!< Render next Page */
            glColor3f(0,0,0);
            glFlush();
        glPopMatrix();
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
    }
    glFlush();
    glutSwapBuffers();
}

/*!
    \fn mykeyboard(unsigned char key, GLint x,GLint y)
    \brief Keyboard interaction
    Callback function for keyboard input interaction.
    unsigned char key -> holds ascii value of key pressed by the user
    x -> x coardinate
    y -> y coordinate
*/
void mykeyboard(unsigned char key, GLint x,GLint y){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    if(key == 'n' || key == 'N')
        pageId = ( pageId + 1 ) % noOfPages; /*!< Goto next Page */
    else if(key == 'p' || key == 'P')
        pageId = ( pageId - 1 ) % noOfPages; /*!< Goto previous Page */
    else if(key == 13){ /*!< Transition from welcome page to main page */
        showWelcomeScreen = 0;
        startFlipping = 0;
        pageAngle = 0;
    }
    else if(key == 'W' || key == 'w'){
        showWelcomeScreen = 1;
        startFlipping = 1;
    }
    else if(key == 'q' || key == 'Q')
        exit(0);
    else if(key == 61)
        speed += 1; /*!< Increase speed */
    else if(key == 45)
        speed -= 1; /*!< Decrease speed */
    glutPostRedisplay();
}

/*!
    \fn myMouseMovement(int x, int y)
    \brief Mouse interaction for movement of Book
*/
void myMouseMovement(int x, int y){
    if(allowBookMovement == 1){
        float centerX = (float)WINDOW_WIDTH / 2.0;
        float centerY = (float)WINDOW_HEIGHT / 2.0;
        float deltaX = (x - centerX)*5;
        float deltaY = (y - centerY)*5;
        translateBookbyX += deltaX / (float)WINDOW_WIDTH;
        translateBookbyY -= deltaY / (float)WINDOW_HEIGHT;
    }
}

/*!
    \fn mymenu(int id)
    \brief Mouse interaction in selection of menu
    Callback function for mouse input interaction.
    int id -> id of the clicked menu item from the user.
*/
void mymenu(int id /*!< Menu id picked by the user */){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    switch(id){
        case 1:
            pageId = ( pageId + 1 ) % noOfPages;break; /*!< Goto next page */
        case 2:
            pageId = ( pageId - 1 ) % noOfPages;break; /*!< Goto previous page */
        case 3:
            flipId = 0;break; /*!< Book Flip */
        case 4:
            flipId = 1;break; /*!< Writing Pad Flip */
        case 5:
            flipId = 2;break; /*!< Notepad Flip*/
        case 6:
            flipId = 3;break; /*!< Special Page Flip */
        case 7:
            viewId = 0;break; /*!< Normal View */
        case 8:
            viewId = 1;break; /*!< Top View */
        case 9:
            viewId = 2;break; /*!< Bottom View */
        case 10:
            viewId = 3;break; /*!< 360 deg Horizontal View */
        case 11:{
            if(startFlipping == 0){ /*!< Flip the Book and start flipping Pages */
                startFlipping = 1;
                pauseFlipping = 0;
            }
            else /*!< Toggle Flipping */
                pauseFlipping = !pauseFlipping;
            break;
        }
        case 12:
            speed += 1;break; /*!< Increase Speed */
        case 13:
            speed -= 1;break; /*!< Decrease Speed */
        case 14:
            viewId = 4;break; /*!< 360 deg Vertical View */
        case 15:
            viewId = 5;break; /*!< 360 deg Horizontal and Vertical View */
        case 16:
            allowBookMovement = !allowBookMovement;
    }
    glutPostRedisplay();
}

/*!
    \fn convertPicstoTextures()
    \brief Load individual images as textures for texture mapping
*/
void convertPicstoTextures(){
    wallTexture = loadTextures(wallImageFilename);
    picTexture = loadTextures(picImageFilename);
    welcomeTexture = loadTextures(welcomeImageFilename);
    tableTexture = loadTextures(tableImageFilename);
    floorTexture = loadTextures(floorImageFilename);
    pic2Texture = loadTextures(pic2ImageFilename);
    wall2Texture = loadTextures(wall2ImageFilename);
}

/*!
    \fn addMenuInteraction()
    \brief Create menu and add entries for mouse interaction
*/
void addMenuInteraction(){
    int mainMenu, flipMenu, viewMenu;
    /*!< Create sub menu for viewing variants */
    viewMenu = glutCreateMenu(mymenu);
    glutAddMenuEntry("Normal View", 7);
    glutAddMenuEntry("Top View", 8);
    glutAddMenuEntry("Bottom View", 9);
    glutAddMenuEntry("Horizontal 360 View", 10);
    glutAddMenuEntry("Vertical 360 View", 14);
    glutAddMenuEntry("H/V 360 View", 15);
    /*!< Create sub menu for flipping variants */
    flipMenu = glutCreateMenu(mymenu);
    glutAddMenuEntry("Book Flip", 3);
    glutAddMenuEntry("Writing Pad Flip", 4);
    glutAddMenuEntry("Notebook Flip", 5);
    glutAddMenuEntry("Special Flip", 6);
    /*!< Create menu and add entries for mouse interaction */
    mainMenu=glutCreateMenu(mymenu);
    glutAddMenuEntry("Toggle Flipping", 11);
    glutAddMenuEntry("Next Page", 1);
    glutAddMenuEntry("Previous Page", 2);
    glutAddSubMenu("View Types", viewMenu);
    glutAddSubMenu("Flip Types", flipMenu);
    glutAddMenuEntry("Speed Up", 12);
    glutAddMenuEntry("Speed Down", 13);
    glutAddMenuEntry("Toggle Book Movement",16);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
/*!
    \fn main(int argc, char **argv)
    Loads individual pictures using libSoil as textures.
    Input interactions used -> mouse, keyboard.
    Add menu entries for mouse interaction.
    Enable lighting and shading.
*/
int main(int argc, char **argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    glutCreateWindow("FlipBook");
    glutReshapeFunc(initReshape);
    glutDisplayFunc(display);
    /*! Load images into textures */
    convertPicstoTextures();
    /*!< Create menu and add entries for mouse interaction */
    addMenuInteraction();
    /*!< Lighting, shading, keyboard, mouse, idle interactions enabled */
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glutKeyboardFunc(mykeyboard);
    glutIdleFunc(idleStateExecute);
    glutPassiveMotionFunc(myMouseMovement);
    glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}
