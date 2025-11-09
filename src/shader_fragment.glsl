#version 330 core

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

#define GLOBALLIGHT 0
#define LAMBERT 1
#define PHONG 2
#define BLINNPHONG 3

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

uniform bool IsGouraudShading;
in vec4 GouraudColor;
uniform int IlluminationModel;

// Identificador que define qual objeto está sendo desenhado no momento
#define SPHERE      0
#define BUNNY       1
#define PLANE       2
#define KART        3
#define RACETRACK   4
#define COIN        5
#define BULLET      6
uniform int object_id;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;
uniform sampler2D TextureImage4;
uniform sampler2D TextureImage5;

void main() {

    if(IsGouraudShading) {
        color = GouraudColor;
    }
    else {

        vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
        vec4 camera_position = inverse(view) * origin;

        vec4 LightPosition = normalize(vec4(0.0,1.0,0.0,0.0));
        vec4 LightDirection = normalize(vec4(0.0, 1.0, 0.0, 0.0));
        vec4 LightL = normalize(position_world - LightPosition);
        float LightAngle = dot(LightL, LightDirection);

        vec4 p = position_world;
        vec4 n = normalize(normal);
        vec4 l = normalize(LightPosition);
        vec4 v = normalize(camera_position - p);
        vec4 r = -l + 2 * n * (dot(n,l));

        // Parâmetros que definem as propriedades espectrais da superfície
        vec3 Kd; // Refletância difusa
        vec3 Ks; // Refletância especular
        vec3 Ka; // Refletância ambiente
        float q; // Expoente especular para o modelo de iluminação de Phong

        // Coordenadas de textura U e V
        float U = 0.0;
        float V = 0.0;

        if ( object_id == SPHERE ) {

            vec4 bbox_center = (bbox_min + bbox_max) / 2.0;
            vec4 pc = position_model - bbox_center;
            float rho = length(pc);
            vec4 pLinha = bbox_center + rho*normalize(pc);
            vec4 pVetor = (pLinha - bbox_center);
            float theta = atan(pVetor.x, pVetor.z);
            float phi = asin(pVetor.y/rho);

            U = (theta + M_PI) / (2*M_PI);
            V = (phi + M_PI_2) / M_PI;

            Kd = texture(TextureImage0, vec2(U,V)).rgb;
            // Ka =
            // Ks =
            // q =
        }
        else if ( object_id == BUNNY ) {

            float minx = bbox_min.x;
            float maxx = bbox_max.x;

            float miny = bbox_min.y;
            float maxy = bbox_max.y;

            float minz = bbox_min.z;
            float maxz = bbox_max.z;

            U = (position_model.x - minx) / (maxx - minx);
            V = (position_model.y - miny) / (maxy - miny);

            Kd = texture(TextureImage0, vec2(U,V)).rgb;
            // Ka =
            // Ks =
            // q =
        }
        else if ( object_id == PLANE ) {
            U = texcoords.x;
            V = texcoords.y;

            Kd = texture(TextureImage1, vec2(U,V)).rgb;
            // Ka =
            // Ks =
            // q =
        }
        else if (object_id == COIN) {

            U = texcoords.x ;
            V = texcoords.y ;

            Kd = texture(TextureImage2, vec2(U,V)).rgb;
            Ka = vec3(0.7, 0.7, 0.0);
            Ks = vec3(0.3, 0.3, 0.3);
            q  = 10.0;

        }
        else if (object_id == KART) {

            U = texcoords.x ;
            V = texcoords.y ;

            Kd = texture(TextureImage3, vec2(U,V)).rgb;
            Ka = vec3(0.0, 0.0, 0.0);
            Ks = vec3(0.0, 0.0, 0.0);
            q  = 10.0;

        }


        //==============================================================================
        // Modelos de Iluminação

        vec3 I = vec3(1.0,1.0,1.0);     // Espectro da fonte de iluminação
        vec3 Ia = vec3(0.2,0.2,0.2);    // Espectro da luz ambiente

        // LAMBERT
        vec3 LambertShading = Kd * I * max(0.0, dot(n,l));

        // PHONG
        vec3 ambient_term = Ka * Ia;
        vec3 phong_specular_term  = Ks * I * pow(max(0.0, dot(r, v)), q);
        vec3 PhongShading = LambertShading + ambient_term + phong_specular_term;

        // BLINN-PHONG
        vec4 h = normalize(v + l);
        vec3 blinn_phong_specular_term = Ks * I * pow(max(0, dot(n, h)), q);
        vec3 BlinnPhongShading = LambertShading + ambient_term + blinn_phong_specular_term;


        if (IlluminationModel == GLOBALLIGHT)
            color.rgb = Kd * (max(0.0, dot(n,l)) + 0.5);
        else if (IlluminationModel == LAMBERT)
            color.rgb = LambertShading;
        else if (IlluminationModel == PHONG)
            color.rgb = PhongShading;
        else if (IlluminationModel == BLINNPHONG)
            color.rgb = BlinnPhongShading;


        color.a = 1;
        color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
    }
} 

