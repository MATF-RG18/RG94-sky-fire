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

        GLuint aeroplane_program = create_program("shaders\\aeroplane.vert", "shaders\\aeroplane.frag");
        GLuint location_projection = glGetUniformLocation(aeroplane_program, "projection_m");
        GLuint location_model = glGetUniformLocation(aeroplane_program, "model_m");
        GLuint location_view = glGetUniformLocation(aeroplane_program, "view_m");



        GLuint location_light_position = glGetUniformLocation(aeroplane_program, "light_position");
        GLuint location_light_colour = glGetUniformLocation(aeroplane_program, "light_colour");



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

        GLuint albedo_texture = load_texture("res\\albedo.jpg");
        GLuint normal_texture = load_texture("res\\normal.jpg");
        GLuint metalness_texture = load_texture("res\\metalness.jpg");



        mesh_t aeroplane_mesh;
        load_wavefront_mesh_with_rotation("res\\model.obj", &aeroplane_mesh, vec3f_create(270.0f, 0.0f, 0.0f));



        mat4f proj_m;
        create_perspective_projection_matrix(&proj_m, 60.0f, (WINDOW_WIDTH * 1.0f) / WINDOW_HEIGHT, 0.01f, 1000.0f);


    /* END OF DEBUG CODE */


    /* main loop */

    clock_t last_frame = clock(), current_frame;
    float elapsed = 0.0f;
    float time = -5.0f;
    glUniform1i(glGetUniformLocation(aeroplane_program, "albedo_sampler"), 0);
    glUniform1i(glGetUniformLocation(aeroplane_program, "normal_sampler"), 1);
    glUniform1i(glGetUniformLocation(aeroplane_program, "metalness_sampler"), 2);

    glEnable(GL_DEPTH_TEST);

    float x = 0, y = 0, z = 0, speed = AEROPLANE_SPEED;

    entity_t aeroplane;
    create_entity(&aeroplane, aeroplane_mesh, 0.0f, 20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    mat4f aeroplane_rotation;
    create_rotation_matrix(&aeroplane_rotation, 0.0f, 0.0f, 0.0f);

    vec3f light_position = vec3f_create(500.0f, 8000.0f, 8000.0f);
    vec3f light_colour = vec3f_create(1.0f, 1.0f, 1.0f);

    float shine_damper = 1.0f; //ME
    float reflectivity = 0; //ME

    int location_shine_damper;
    int location_reflectivity;


    mesh_t plane_mesh = generate_plane_mesh(200, 200, 200.0f, 200.0f);
    entity_t terrain_tile;
    create_entity(&terrain_tile, plane_mesh, 0, 0, 0, 0, 0, 0, 1.0f);

    GLuint terrain_program = create_program("shaders\\terrain.vert", "shaders\\terrain.frag");

    GLuint location_projection_terrain = glGetUniformLocation(terrain_program, "projection_m");
    GLuint location_model_terrain = glGetUniformLocation(terrain_program, "model_m");
    GLuint location_view_terrain = glGetUniformLocation(terrain_program, "view_m");
    GLuint location_light_colour_terrain = glGetUniformLocation(terrain_program, "light_colour");
    GLuint location_light_position_terrain = glGetUniformLocation(terrain_program, "light_position");
    GLuint location_height_factor_terrain = glGetUniformLocation(terrain_program, "height_factor");
    GLuint location_player_position_terrain = glGetUniformLocation(terrain_program, "player_position");

    GLuint heightmap = load_texture("res\\heightmap.jpg");

    glUniform1i(glGetUniformLocation(terrain_program, "height_map"), 0);


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

        if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            speed = AEROPLANE_SPEED * 5;
        }
        else
        {
            speed = AEROPLANE_SPEED;
        }

        //printf("Forward: (%5.2f, %5.2f, %5.2f)\n", vforward.x, vforward.y, vforward.z);
        aeroplane.position = vec3f_add(aeroplane.position, vec3f_scalar_product(vforward, elapsed * speed));

        float inversion = 1.0f;
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            mat4f rotm;
            create_rotation_matrix(&rotm, 0.0f, 0.0, 30.0f * elapsed * inversion);
            aeroplane_rotation = matrix_multiply(aeroplane_rotation, rotm);
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            mat4f rotm;
            create_rotation_matrix(&rotm, 0.0f, 0.0, -30.0f * elapsed * inversion);
            aeroplane_rotation = matrix_multiply(aeroplane_rotation, rotm);
        }

        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            mat4f rotm;
            create_rotation_matrix(&rotm, 0.0f, 30.0f * elapsed * inversion, 0.0f);
            aeroplane_rotation = matrix_multiply(aeroplane_rotation, rotm);
        }

        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            mat4f rotm;
            create_rotation_matrix(&rotm, 0.0f, -30.0f * elapsed * inversion, 0.0f);
            aeroplane_rotation = matrix_multiply(aeroplane_rotation, rotm);
        }

        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            mat4f rotm;
            create_rotation_matrix(&rotm, 30.0f * elapsed * inversion, 0.0f, 0.0f);
            aeroplane_rotation = matrix_multiply(aeroplane_rotation, rotm);
        }

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
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
        //render terrain

        glUseProgram(terrain_program);
        glBindVertexArray(terrain_tile.mesh.vao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, heightmap);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        create_model_matrix_entity(&terrain_tile.model_m, &terrain_tile);


        glUniformMatrix4fv(location_projection_terrain, 1, GL_TRUE, proj_m.data);

        glUniformMatrix4fv(location_model_terrain, 1, GL_TRUE, terrain_tile.model_m.data);
        glUniformMatrix4fv(location_view_terrain, 1, GL_TRUE, view_m.data);

        glUniform3f(location_light_position_terrain, light_position.x, light_position.y, light_position.z);
        glUniform3f(location_light_colour_terrain, light_colour.x, light_colour.y, light_colour.z);
        glUniform3f(location_player_position_terrain, aeroplane.position.x, aeroplane.position.y, aeroplane.position.z);


        float elevation_factor = MAXM(0.0, time * 3.0f);
        elevation_factor = MINM(elevation_factor, 15.0f);

        glUniform1f(location_height_factor_terrain, elevation_factor);

        glDrawElements(GL_TRIANGLES, terrain_tile.mesh.vertex_count, GL_UNSIGNED_INT, NULL);


        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);



        //render aeroplanes
        glUseProgram(aeroplane_program);

        glBindVertexArray(aeroplane.mesh.vao);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedo_texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normal_texture);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, metalness_texture);


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
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 0);
        glfwSwapBuffers(window);
    }


    glfwTerminate();
    return 0;
}
