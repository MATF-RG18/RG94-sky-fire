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

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Sky Fire", NULL, NULL);
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
        GLuint location_view = glGetUniformLocation(program, "view_m");

        GLuint location_light_position = glGetUniformLocation(program, "light_position");
        GLuint location_light_colour = glGetUniformLocation(program, "light_colour");



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

        mesh_t aeroplane_mesh;
        load_wavefront_mesh_with_rotation("res\\model.obj", &aeroplane_mesh, vec3f_create(270.0f, 0.0f, 0.0f));

        mat4f proj_m;
        create_perspective_projection_matrix(&proj_m, 60.0f, (WINDOW_WIDTH * 1.0f) / WINDOW_HEIGHT, 0.01f, 1000.0f);


    /* END OF DEBUG CODE */


    /* main loop */

    clock_t last_frame = clock(), current_frame;
    float elapsed = 0.0f;
    float time = 0.0f;
    glUniform1i(glGetUniformLocation(program, "albedo"), 0);
    glUniform1i(glGetUniformLocation(program, "noise"), 1);

    glEnable(GL_DEPTH_TEST);

    float x = 0, y = 0, z = 0, speed = 10.0f;

    entity_t aeroplane;
    create_entity(&aeroplane, aeroplane_mesh, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    mat4f aeroplane_rotation;
    create_rotation_matrix(&aeroplane_rotation, 0.0f, 0.0f, 0.0f);

    vec3f light_position = vec3f_create(500.0f, 8000.0f, 8000.0f);
    vec3f light_colour = vec3f_create(1.0f, 1.0f, 1.0f);

    while(!glfwWindowShouldClose(window))
    {

        glfwPollEvents();

        current_frame = clock();

        elapsed = ((float)(current_frame - last_frame)) / ((float)CLOCKS_PER_SEC);

        last_frame = current_frame;

        //printf("elapsed: %5.2f fps: %5.2f\n", elapsed, 1.0f / elapsed);

        time += elapsed;

        //update

        mat4f model_m;

        vec3f vforward = matrix_vector_multiply(aeroplane_rotation, vec3f_create(0.0f, 0.0f, -1.0f));
        vec3f vup = matrix_vector_multiply(aeroplane_rotation, vec3f_create(0.0f, 1.0f, 0.0f));
        vec3f vback = vec3f_scalar_product(vforward, -1.0f);

        //printf("Forward: (%5.2f, %5.2f, %5.2f)\n", vforward.x, vforward.y, vforward.z);

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            aeroplane.position = vec3f_add(aeroplane.position, vec3f_scalar_product(vforward, elapsed * speed));
        }
        float inversion = 1.0f;
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            aeroplane.position = vec3f_subtract(aeroplane.position, vec3f_scalar_product(vforward, elapsed * speed));
            inversion = -1.0f;
        }

        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            mat4f rotm;
            create_rotation_matrix(&rotm, 0.0f, 30.0f * elapsed * inversion, 0.0f);
            aeroplane_rotation = matrix_multiply(aeroplane_rotation, rotm);
        }

        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            mat4f rotm;
            create_rotation_matrix(&rotm, 0.0f, -30.0f * elapsed * inversion, 0.0f);
            aeroplane_rotation = matrix_multiply(aeroplane_rotation, rotm);
        }

        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            mat4f rotm;
            create_rotation_matrix(&rotm, 30.0f * elapsed * inversion, 0.0f, 0.0f);
            aeroplane_rotation = matrix_multiply(aeroplane_rotation, rotm);
        }

        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            mat4f rotm;
            create_rotation_matrix(&rotm, -30.0f * elapsed * inversion, 0.0f, 0.0f);
            aeroplane_rotation = matrix_multiply(aeroplane_rotation, rotm);
        }

        //create_model_matrix_entity(&model_m, &aeroplane);
        mat4f transl;
        create_translation_matrix(&transl, aeroplane.position.x, aeroplane.position.y, aeroplane.position.z);
        mat4f scal;
        create_scale_matrix(&scal, aeroplane.scale, aeroplane.scale, aeroplane.scale);
        model_m = matrix_multiply(aeroplane_rotation, scal);
        model_m = matrix_multiply(transl, model_m);


        mat4f view_m;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        camera_t c;

        c.pos = vec3f_add(aeroplane.position, vec3f_scalar_product(vback, 4.0f));
        c.pos = vec3f_add(c.pos, vec3f_scalar_product(vup, 2.0f));

        camera_loot_at(aeroplane.position, c.pos, &c);

        printf("Camera data: pos(%5.2f, %5.2f, %5.2f) yaw: %5.2f  pitch: %5.2f  roll: %5.2f\n", c.pos.x, c.pos.y, c.pos.z, c.yaw, c.pitch, c.roll);
        create_camera_view_matrix(&view_m, c);
        //matrix_set_to_identity(&view_m);
        //create_view_matrix(&view_m, x, 0, y, 0, z * 25.0f, 0);
        mat4f_show(view_m);
        process_input(window);



        mat4f model2;
        create_model_matrix(&model2, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f);


        //draw

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);



        glBindVertexArray(aeroplane.mesh.vao);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedo_texture);


        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, noise_texture);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);


        glUniformMatrix4fv(location_projection, 1, GL_TRUE, proj_m.data);

        glUniformMatrix4fv(location_model, 1, GL_TRUE, model_m.data);
        glUniformMatrix4fv(location_view, 1, GL_TRUE, view_m.data);

        glUniform3f(location_light_position, light_position.x, light_position.y, light_position.z);
        glUniform3f(location_light_colour, light_colour.x, light_colour.y, light_colour.z);

        glDrawArrays(GL_TRIANGLES, 0, aeroplane.mesh.vertex_count);

        glUniformMatrix4fv(location_model, 1, GL_TRUE, model2.data);
        glDrawArrays(GL_TRIANGLES, 0, aeroplane.mesh.vertex_count);

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
