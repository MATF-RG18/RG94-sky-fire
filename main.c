#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <helpers.h>
#include <models.h>

#include <string.h>

#include <matrices.h>



#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <constants.h>

//OBJEKAT

const struct aiScene* scene = NULL;
GLint scene_list = 0;
struct aiVector3D scene_min, scene_max, scene_center;

static float rotation_angle = 0.f;

float vertices_triangle[] = {
     0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,       1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,       1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,       0.0f, 0.0f,// bottom left
    -0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,       0.0f, 1.0f // top left
};
unsigned int indices_triangle[] = {
    0, 1, 3,
    1, 2, 3
};

int lines = 0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void set_all_glfw_callbacks(GLFWwindow *window)
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}


void process_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }


    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

}

int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "main", NULL, NULL);
    if(window == NULL)
    {
        fprintf(stderr, "Failed to create GLFW window!\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initiate GLAD!\n");
        glfwTerminate();
        return -1;
    }

    set_all_glfw_callbacks(window);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    /* DEBUG CODE, DELETE LATER */

        GLuint program = create_program("vert.glsl", "frag.glsl");
        GLuint location_projection = glGetUniformLocation(program, "projection_m");
        GLuint location_model = glGetUniformLocation(program, "model_m");




        GLuint triangle_vbo;
        glGenBuffers(1, &triangle_vbo);

        GLuint triangle_vao;
        glGenVertexArrays(1, &triangle_vao);

        GLuint triangle_ebo;
        glGenBuffers(1, &triangle_ebo);

        glBindVertexArray(triangle_vao);
        glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle), vertices_triangle, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 3 * sizeof(float));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 6 * sizeof(float));


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_triangle), indices_triangle, GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        int width, height, nrChannels;
        unsigned char *data = stbi_load("res\\albedo.jpg", &width, &height, &nrChannels, 0);
        GLuint albedo_texture;
        glGenTextures(1, &albedo_texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedo_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        data = stbi_load("res\\noise.jpg", &width, &height, &nrChannels, 0);
        GLuint noise_texture;
        glGenTextures(1, &noise_texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, noise_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        mesh model;
        load_wavefont_mesh("res\\model.obj", &model);

        mat4f proj_m;
        create_perspective_projection_matrix(&proj_m, 80.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 0.05f, 1000.0f);


    /* END OF DEBUG CODE */


    /* main loop */

    clock_t last_frame = clock(), current_frame;
    float elapsed = 0.0f;
    float time = -10.0f;
    glUniform1i(glGetUniformLocation(program, "albedo"), 0);
    glUniform1i(glGetUniformLocation(program, "noise"), 1);

    glEnable(GL_DEPTH_TEST);

    float x = 0, y = 0, z = 1.75f;

    while(!glfwWindowShouldClose(window))
    {

        glfwPollEvents();

        current_frame = clock();

        elapsed = ((float)(current_frame - last_frame)) / ((float)CLOCKS_PER_SEC);

        last_frame = current_frame;

        printf("elapsed: %5.2f fps: %5.2f\n", elapsed, 1.0f / elapsed);

        time += elapsed;

        //update

        mat4f model_m;

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            y += elapsed;
        }

        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            y -= elapsed;
        }

        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            x -= elapsed;
        }

        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            x += elapsed;
        }

        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            z -= elapsed;
        }

        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            z += elapsed;
        }

        memset(model_m.data, 0, 4 * 4 * sizeof(float));
        model_m.data[0][0] = 1.0f;
        model_m.data[1][1] = 1.0f;
        model_m.data[2][2] = 1.0f;
        model_m.data[3][3] = 1.0f;

        model_m.data[0][3] = x;
        model_m.data[1][3] = y;
        model_m.data[2][3] = z;


        process_input(window);



        //draw

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);



        glBindVertexArray(model.vao);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedo_texture);


        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, noise_texture);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);


        glUniformMatrix4fv(location_projection, 1, GL_FALSE, proj_m.data);

        glUniformMatrix4fv(location_model, 1, GL_TRUE, model_m.data);
        glDrawArrays(GL_TRIANGLES, 0, model.vertex_count);


        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glfwSwapBuffers(window);
    }


    glfwTerminate();
    return 0;
}
