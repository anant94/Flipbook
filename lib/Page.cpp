#include "./headers/Page.h"

/*!
    \fn Page::Page(GLint type, GLfloat x, GLfloat y, GLfloat z, char text[])
    \brief Constructor
    Sets x, y, z coordinate for the Page.
    text -> text to be rendered in that Page.
    type -> type of Page.
*/
Page::Page(GLint type, GLfloat x, GLfloat y, GLfloat z, char text[]){
    init(type, x, y, z);
    this->text=text;
}

/*!
    \fn Page::Page(GLint type, GLfloat x, GLfloat y, GLfloat z, void (*pageContent)(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat))
    \brief Constructor
    Sets x, y, z coordinate for the Page.
    type -> type of Page.
    pageContent -> function pointer which renders the drawing from here to the Page.
*/
Page::Page(GLint type, GLfloat x, GLfloat y, GLfloat z, void (*pageContent)(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat)){
    init(type, x, y, z);
    if(type == PAGE_TYPE_DRAWING)
    {
        this->pageContent = pageContent;
    }
}

/*!
    \fn Page::init(GLint type, GLfloat x, GLfloat y, GLfloat z)
    \brief Initialisation of data members
    Constructor which initialises all the data members required for Page object
*/
void Page::init(GLint type, GLfloat x, GLfloat y, GLfloat z){
    this->x = x;
    this->y = y;
    this->z = z;
    this->type = type;
    this->width = PAGE_WIDTH;
    this->height = PAGE_HEIGHT;
    this->marginWidth = MARGIN_WIDTH;
    this->marginHeight = MARGIN_HEIGHT;
    this->lineHeight = LINE_HEIGHT;
    this->pageContent = NULL;
    this->text = NULL;
}

/*!
    \fn Page::renderPage()
    \brief Renders the specified Page
    Renders the Page onto the screen after setting the border, margin.
    text pages -> sets lines for the page and render the specified text.
    drawing pages -> calls the user defined function to draw inside the Page.
*/
void Page::renderPage(){
    glColor3f(0.0,0.0,0.0);
    setBorder();
    setMargin();
    if(type == PAGE_TYPE_TEXT && text != NULL){
        setPageLines();
        renderText();
    }
    else if(type == PAGE_TYPE_DRAWING && pageContent != NULL)
        pageContent(x + marginWidth, y, z, x + width, y + height - marginHeight);
}

/*!
    \fn Page::renderText()
    \brief Renders the specified text on the Page.
    Renders the text onto the page after calculating the offset value required 
    for placing the character.
    On a newline entry goto the next line and so on.
*/
void Page::renderText(){
    char ch;
    int i=0, lines=0, characterHeight = 130, characterWidth, len=strlen(text);
    int bufferWidth = 5, bufferHeight = 5;
    GLfloat ratio = LINE_HEIGHT/130.0, sum = bufferWidth;
    for(i=0;i<len;i++){
        glPushMatrix();
            ch = text[i];
            characterWidth = glutStrokeWidth(GLUT_STROKE_ROMAN, ch);
            characterHeight = characterHeight * ratio;
            characterWidth = characterWidth * ratio;
            /*! Goto the next line for placing the character */
            if((sum + characterWidth > PAGE_WIDTH - MARGIN_WIDTH) || ch == '\n'){
                lines++;
                sum = characterWidth;
                glTranslatef(BOOK_BORDER_SIZE + MARGIN_WIDTH, 
                    BOOK_BORDER_SIZE + PAGE_HEIGHT - MARGIN_HEIGHT - (lines + 1) * LINE_HEIGHT + bufferWidth,
                    z);
            }
            /*! Place the character in the current line */
            else{
                glTranslatef(BOOK_BORDER_SIZE + MARGIN_WIDTH + sum, 
                    BOOK_BORDER_SIZE + PAGE_HEIGHT - MARGIN_HEIGHT - (lines + 1) * LINE_HEIGHT + bufferWidth,
                    z);
                    sum += characterWidth;
            }
            glScalef(ratio, ratio, 1);
            glutStrokeCharacter(GLUT_STROKE_ROMAN, ch); /*!< Place the character */
            glFlush();
        glPopMatrix();
    }
    
}

/*!
    \fn Page::setBorder()
    \brief Set Page border on the screen.
    Set page border which is rendered onto the screen
    from the config/constants file.
*/
void Page::setBorder(){
    GLfloat xLimit = x + width;
    GLfloat yLimit = y + height;
    GLfloat zLimit = z - PAGE_THICKNESS;
    glColor3f(0,0,0);
    cubeConstruction(x, y, z, xLimit, yLimit, zLimit);
    glColor3f(1,1,1);
    glBegin(GL_POLYGON);
        glVertex3f(x, y, z-PAGE_THICKNESS);
        glVertex3f(x + width, y, z-PAGE_THICKNESS);
        glVertex3f(x + width, y + height, z-PAGE_THICKNESS);
        glVertex3f(x, y + height, z-PAGE_THICKNESS);
    glEnd();
}

/*!
    \fn Page::setMargin()
    \brief Set Page Margin
    Set margin for the page which is to be rendered onto the screen
    from the config/constants file.
*/
void Page::setMargin(){
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
        glVertex3f(x + marginWidth, y, z);
        glVertex3f(x + marginWidth, y + height, z);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(x, y + height - marginHeight, z);
        glVertex3f(x + width, x + height - marginHeight, z);
    glEnd();
}

/*!
    \fn Page::setPageLines()
    \brief Set Page lines for text Page.
    Render pagelines for the Page from the margin where the line height is given
    by LINE_HEIGHT.
*/
void Page::setPageLines(){
    int index=0;
    float pageBodyStartx = x + marginWidth;
    float pageBodyStarty = y + height - marginHeight;
    float pageBodyEndx = x + width;
    glColor3f(1.0,0.0,0.0);
    for(index=1;index<MAX_LINES && index<((pageBodyStarty-y)/(lineHeight));index++)
    {
        lines[index] = (pageBodyStarty - (LINE_HEIGHT * index));
        glBegin(GL_LINES);
            glVertex3f(pageBodyStartx, lines[index],z);
            glVertex3f(x + width, lines[index],z);
        glEnd();
    }
    glColor3f(0.0,0.0,0.0);
    noOfLines=index-1;
}

/*!
    \fn squareConstruction(GLint a,GLint b,GLint c,GLint d,GLfloat vertices[][3])
    \brief construct a square
    Construction of a individual faces based on the input of each of the vertices to form a cube
*/
void squareConstruction(GLint a,GLint b,GLint c,GLint d,GLfloat vertices[][3]){
    glBegin(GL_LINE_LOOP);
        glVertex3fv(vertices[a]);
        glNormal3fv(vertices[a]);
        glVertex3fv(vertices[b]);
        glNormal3fv(vertices[b]);
        glVertex3fv(vertices[c]);
        glNormal3fv(vertices[c]);
        glVertex3fv(vertices[d]);
        glNormal3fv(vertices[d]);
    glEnd();
}

/*!
    \fn cubeConstruction(GLfloat x, GLfloat y, GLfloat z, GLfloat xLimit, GLfloat yLimit, GLfloat zLimit)
    \brief Construct individual faces for the cube.
    Construction of a cube called from the square construction
*/
void cubeConstruction(GLfloat x, GLfloat y, GLfloat z, GLfloat xLimit, GLfloat yLimit, GLfloat zLimit){
    GLfloat vertices[][3]={
        {x, y, z}, {xLimit, y, z}, {xLimit, yLimit, z}, {x, yLimit, z},
        {x, y, zLimit}, {xLimit, y, zLimit}, {xLimit, yLimit, zLimit}, {x, yLimit, zLimit}
    };
    squareConstruction(0,3,2,1,vertices);
    squareConstruction(2,3,7,6,vertices);
    squareConstruction(0,4,7,3,vertices);
    squareConstruction(1,2,6,5,vertices);
    squareConstruction(4,5,6,7,vertices);
    squareConstruction(0,1,5,4,vertices);
}