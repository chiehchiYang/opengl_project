// C14069018 楊傑祺 opengl hw3
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SOIL2/SOIL2.h"


unsigned int loadTexture(char const * path);
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

Shader shader;////////shader object
int jump_state = 0;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

// Camera

Camera camera( glm::vec3( 1.5f, 0.5f, 3.5f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;



GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

class MatrixStack {
     int    index;
     int    size;
    glm::mat4*  matrices;
     
 public:
     MatrixStack(int numMatrices = 32) :index(0), size(numMatrices)
     {
         matrices = new glm::mat4[numMatrices];
     }
     
     ~MatrixStack()
     {
         delete[]matrices;
     }
     
    void push(const glm::mat4& m) {
         assert(index + 1 < size);
         matrices[index++] = m;
     }
     
    glm::mat4& pop(void) {
         assert(index - 1 >= 0);
         index--;
         return matrices[index];
     }
};


class mymodels{
public:
    mymodels(){
        model_view = glm::mat4(1.0f);
        move_view = glm::mat4(1.0f);
        jump_view = glm::mat4(1.0f);
        
    }
    void setup(){
        head.Setup("res/models/head/head.obj");
        body.Setup("res/models/body/body.obj" );
        
        
        leg_right.Setup( "res/models/leg_left/leg.obj" );
        leg_left.Setup( "res/models/leg_left/leg.obj" );
        
        arm_right.Setup( "res/models/arm_left/left_arm.obj" );
        
        arm_left.Setup( "res/models/arm_left/left_arm.obj" );
        
        drowned.Setup( "res/models/drowned/drowned.obj" );
        glm:: mat4 m(1.0f);
        m = glm::scale( m, glm::vec3( 0.1f, 0.1f, 0.1f ) );
        
       
        drowned.transform = m;
        //drowned.transform = glm:: rotate(drowned.transform, glm::radians(50.0f), glm::vec3(1.0, 0.0, 1.0));
        drowned.transform = glm:: translate( drowned.transform , glm::vec3(0.0f, 5.1f, 1.2f));
        //drowned.transform = glm:: rotate(drowned.transform, glm::radians(0.0f), glm::vec3(0.0, 0.0, -1.0));
        
        drowned.transform = glm:: translate( drowned.transform , glm::vec3(0.0f, -2.5f, 0.0f));
        
        
        drowned.tmp = drowned.transform;
        
        
        arm_right.transform = m;
        arm_right.transform = glm:: rotate(arm_right.transform, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        
        arm_right.transform= glm:: translate( arm_right.transform, glm::vec3(-1.2f, 4.8f, 0.0f));
        
        arm_right.tmp = arm_right.transform;
        
        head.transform = m;
        head.transform = glm:: rotate(head.transform, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        
        head.transform = glm:: translate(head.transform, glm::vec3(0.0f, 7.1f, 0.0f));
        
        
        head.tmp = head.transform;
        
        
        body.transform = m;
        body.transform = glm:: rotate(body.transform, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        
        body.transform = glm:: translate(body.transform, glm::vec3(0.0f, 4.8f, 0.0f));
        
        body.tmp = body.transform;
        
        leg_left.transform = m;
         leg_left.transform = glm:: rotate(leg_left.transform, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        
        leg_left.transform = glm:: translate(leg_left.transform, glm::vec3(0.4f, 2.4f, 0.0f));
       
        leg_left.tmp = leg_left.transform;
        
        
        
        leg_right.transform = m;
        leg_right.transform = glm:: rotate(leg_right.transform, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        
        leg_right.transform = glm:: translate(leg_right.transform, glm::vec3(-0.4f, 2.4f, 0.0f));
        
        leg_right.tmp = leg_right.transform;
        
        
        
        arm_left.transform = m;
        arm_left.transform = glm:: rotate(arm_left.transform, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        
        arm_left.transform= glm:: translate( arm_left.transform, glm::vec3(1.2f, 4.8f, 0.0f));
        
        arm_left.tmp = arm_left.transform;
        
        

    }
    // nothing but drawing
    void right_hand_rise(){
        right_arm_angle -=0.1f;
        glm:: mat4 instance1(1.0f) ;
        instance1 = glm:: rotate(instance1, glm::radians(right_arm_angle), glm::vec3(1.0, 0.0, 0.0));
        glm:: mat4 instance2(1.0f) ;
        
        
        instance2 = glm::scale( instance2, glm::vec3( 0.1f, 0.1f, 0.1f ) );
        instance2 = glm:: translate( instance2 , glm::vec3(0.0f, 4.8f, 1.2f));
        instance2 = glm:: rotate(instance2, glm::radians(right_arm_angle), glm::vec3(0.0, 0.0, -1.0));
        instance2 = glm:: translate( instance2 , glm::vec3(0.0f, -2.4f, 0.0f));
        
        arm_right.transform = arm_right.tmp;
        
        arm_right.transform *= instance1 ;
        drowned.transform = instance2 ;
        

        
    }
    void right_hand_down(){
        right_arm_angle +=0.1f;
        glm:: mat4 instance1(1.0f) ;
        instance1 = glm:: rotate(instance1, glm::radians(right_arm_angle), glm::vec3(1.0, 0.0, 0.0));
        glm:: mat4 instance2(1.0f) ;
        
        
        instance2 = glm::scale( instance2, glm::vec3( 0.1f, 0.1f, 0.1f ) );
        instance2 = glm:: translate( instance2 , glm::vec3(0.0f, 4.8f, 1.2f));
        instance2 = glm:: rotate(instance2, glm::radians(right_arm_angle), glm::vec3(0.0, 0.0, -1.0));
        instance2 = glm:: translate( instance2 , glm::vec3(0.0f, -2.4f, 0.0f));
        
        arm_right.transform = arm_right.tmp;
        
        arm_right.transform *= instance1 ;
        drowned.transform = instance2 ;
        
        
        
    }
    
    void left_hand_rise(){
        left_arm_angle-=0.1f;
        glm:: mat4 instance1(1.0f) ;
        
        arm_left.transform = arm_left.tmp;
        
        
        instance1 = glm:: rotate(instance1, glm::radians(left_arm_angle), glm::vec3(1.0, 0.0, 0.0));
        arm_left.transform *= instance1 ;
        //cout << left_arm_angle << endl;
        
        
    }
    void left_hand_down(){
       left_arm_angle+=0.1f;
        glm:: mat4 instance1(1.0f) ;
        
        arm_left.transform = arm_left.tmp;
        
        
        instance1 = glm:: rotate(instance1, glm::radians(left_arm_angle), glm::vec3(1.0, 0.0, 0.0));
        arm_left.transform *= instance1 ;
        //cout << left_arm_angle
        
        
        
        
    }

    void walking_fordward(){
        mvstack.push(model_view);

        z_value = 0.001f;
        //move_view = glm::mat4(1.0f);
       
        move_view = translate( move_view, glm::vec3( z_value * cos( turn_left_right_angle), 0.0f, z_value * sin(turn_left_right_angle)));
        //cout <<  turn_left_right_angle << endl;
        //move_view_tmp = move_view;
        
        //cout << (int)turn_left_right_angle << "angle\n";
        
        glm:: mat4 instance1(1.0f) ;
        glm:: mat4 instance2(1.0f) ;
        leg_left.transform = leg_left.tmp;
        leg_right.transform = leg_right.tmp;
        
        if(0 == left_arm_state){
            instance1 = glm:: rotate(instance1, glm::radians(angle), glm::vec3(1.0, 0.0, 0.0));
            leg_left.transform *= instance1 ;
            
            instance2 = glm:: rotate(instance2, glm::radians(-1 * angle), glm::vec3(1.0, 0.0, 0.0));
            
            leg_right.transform *= instance2;
            
            
        }else if (1 == left_arm_state){
            instance1 = glm:: rotate(instance1, glm::radians(angle), glm::vec3(1.0, 0.0, 0.0));
            leg_left.transform *= instance1 ;
            
            instance2 = glm:: rotate(instance2, glm::radians(-1 * angle), glm::vec3(1.0, 0.0, 0.0));
            
            leg_right.transform *= instance2;
        }
        
        
        if(0 == left_arm_state){
            angle+=0.1f;
            if(angle >= 25.0f){
                left_arm_state++;
                left_arm_state= left_arm_state %2;
            }
            
        }else if (1 == left_arm_state){
            angle-=0.1f;
            if(angle <= -25.0f){
                left_arm_state++;
                left_arm_state= left_arm_state %2;
            }
        }
        
        model_view = mvstack.pop();
        
    }
    void walking_backword(){
        mvstack.push(model_view);
        
        z_value = -0.001f;
        
        move_view = translate( move_view, glm::vec3( z_value * cos( turn_left_right_angle  ), 0.0f, z_value * sin(turn_left_right_angle)));
        //move_view_tmp = move_view;
        
        //cout << (int)turn_left_right_angle << "angle\n";
        
        glm:: mat4 instance1(1.0f) ;
        glm:: mat4 instance2(1.0f) ;
        leg_left.transform = leg_left.tmp;
        leg_right.transform = leg_right.tmp;
        
        if(1 == left_arm_state){
            instance1 = glm:: rotate(instance1, glm::radians(angle), glm::vec3(1.0, 0.0, 0.0));
            leg_left.transform *= instance1 ;
            
            instance2 = glm:: rotate(instance2, glm::radians(-1 * angle), glm::vec3(1.0, 0.0, 0.0));
            
            leg_right.transform *= instance2;
            
            
        }else if (0 == left_arm_state){
            instance1 = glm:: rotate(instance1, glm::radians(angle), glm::vec3(1.0, 0.0, 0.0));
            leg_left.transform *= instance1 ;
            
            instance2 = glm:: rotate(instance2, glm::radians(-1 * angle), glm::vec3(1.0, 0.0, 0.0));
            
            leg_right.transform *= instance2;
        }
        
        
        if(0 == left_arm_state){
            angle+=0.1f;
            if(angle >= 25.0f){
                left_arm_state++;
                left_arm_state= left_arm_state %2;
            }
            
        }else if (1 == left_arm_state){
            angle-=0.1f;
            if(angle <= -25.0f){
                left_arm_state++;
                left_arm_state= left_arm_state %2;
            }
        }
        
        model_view = mvstack.pop();
        
    }
    
    void jump(){
        left_hand_rise();
        walking_fordward();
        if(jump_status == 0 ){
            
            jump_z_value += 0.00007f;
            move_view = translate( move_view, glm::vec3( 0.0f, 0.001f,0.0f));
            if(jump_z_value > 0.01f)
                jump_status = 1;
            
            
        }else if (jump_status == 1 ){
            jump_z_value -= 0.00008f;
            move_view = translate( move_view, glm::vec3( 0.0f, -0.001f,0.0f));
            if(jump_z_value < 0.00f){
                jump_state = 0 ;
                jump_status = 0;
                jump_z_value = 0.0f;
            }
        }
    }
    void turn_left(){
        //turn_left_right_angle
        turn_left_right_angle += 0.5f  ;
        model_view = glm:: mat4(1.0f);
        model_view = glm:: rotate(model_view, glm::radians(turn_left_right_angle), glm::vec3(0.0, 1.0, 0.0));
        
    }
    void turn_right(){
        turn_left_right_angle -= 0.5f;
        model_view = glm:: mat4(1.0f);
        model_view = glm:: rotate(model_view, glm::radians(turn_left_right_angle), glm::vec3(0.0, 1.0, 0.0));
        
    }


    void doDraw(){
        
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( move_view * model_view * body.transform ) );
        body.Draw(shader);
        
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( move_view * model_view * leg_right.transform ) );
        leg_right.Draw(shader);
        
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr(move_view * model_view * leg_left.transform ) );
        leg_left.Draw(shader);
        
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr(move_view * model_view * arm_left.transform ) );
        arm_left.Draw(shader);
        
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( move_view * model_view * arm_right.transform ) );
        arm_right.Draw(shader);
        
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr(move_view *  model_view * head.transform ) );
        head.Draw(shader);
        
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( move_view * model_view * drowned.transform ) );
        drowned.Draw(shader);
    }
    
private:
    Model head,body,leg_right,leg_left,arm_right,arm_left,drowned;
    MatrixStack  mvstack;
    glm::mat4  model_view;
    glm::mat4 move_view;
    glm::mat4 move_view_tmp;
    glm::mat4 jump_view;
    float angle = 0.0f;
    float left_arm_angle = 0.0f;
    float right_arm_angle = 0.0f;
    float drowned_angle = 0.0f;
    
    
    int left_arm_state = 1;
    
    float turn_left_right_angle = 0.0f;
    float z_value = 0.0f;
    
    float jump_z_value = 0.0f;
    int jump_status = 0;
    


    
};

mymodels mymodel;
int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    ////transparent problem?
    glEnable(GL_BLEND);
    
    //glBlendEquation (GL_FUNC_ADD);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    mymodel.setup();
    
    // Setup and compile our shaders
    shader.Setup( "res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag" );
    

    // Draw in wireframe
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
 
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    // Game loop
    while( !glfwWindowShouldClose( window ) )
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Check and call events
        glfwPollEvents( );
        DoMovement( );
        // Clear the colorbuffer
        glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        shader.Use( );
        
        glm::mat4 view = camera.GetViewMatrix( );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
        
        // Draw the loaded model
        if(jump_state == 1){
            mymodel.jump();
            //cout << "test\n";
        }
        mymodel.doDraw();
        
        
        //mymodel.walking_fordward();
        
        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement( )
{
    
    if (  keys[GLFW_KEY_UP] )
    {
        mymodel.walking_fordward();
        //mymodel.left_hand_down();
    }
    
    if (  keys[GLFW_KEY_DOWN] )
    {
        //mymodel.left_hand_rise();
        mymodel.walking_backword();
    }
    if (  keys[GLFW_KEY_SPACE] )
    {
        //mymodel.jump();
        if(jump_state !=1)
            jump_state = 1;
    }
    
    
    if (  keys[GLFW_KEY_LEFT] )
    {
        mymodel.turn_left();
    }
    
    if (  keys[GLFW_KEY_RIGHT] )
    {
        mymodel.turn_right();
    }
    if ( keys[GLFW_KEY_Z]  )
    {
        mymodel.left_hand_rise();
    }
    if ( keys[GLFW_KEY_X]  )
    {
        mymodel.left_hand_down();
    }
    if( keys[GLFW_KEY_C] ){
        mymodel.right_hand_rise();
    }
    if( keys[GLFW_KEY_V] ){
        mymodel.right_hand_down();
    }
    //
    
    
    
    // Camera controls
    if ( keys[GLFW_KEY_W]  )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S]  )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D]  )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }
    
    
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

