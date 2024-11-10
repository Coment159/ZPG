#ifndef TRANSFORMATION_N
#define TRANSFORMATION_N

//Include GLM  
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <memory>


class Transformation {
public:
    virtual ~Transformation() = default;

    virtual glm::mat4 getMatrix() const = 0;

    virtual std::unique_ptr<Transformation> clone() const = 0;
};


class Translation : public Transformation {
public:
    Translation(float x, float y, float z) : translationVector(x, y, z) {}

    glm::mat4 getMatrix() const override {
        return glm::translate(glm::mat4(1.0f), translationVector);
    }
    std::unique_ptr<Transformation> clone() const override {
        return std::make_unique<Translation>(*this);
    }

private:
    glm::vec3 translationVector;
};

class Rotation : public Transformation {
public:
    Rotation(float angle, float x, float y, float z) : angle(angle), axis(x, y, z) {}

    glm::mat4 getMatrix() const override {
        return glm::rotate(glm::mat4(1.0f), angle, axis);
    }

    std::unique_ptr<Transformation> clone() const override {
        return std::make_unique<Rotation>(*this);
    }

private:
    float angle;
    glm::vec3 axis;
};

class Scaling : public Transformation {
public:
    Scaling(float scale) : scaleVector(scale, scale, scale) {}

    glm::mat4 getMatrix() const override {
        return glm::scale(glm::mat4(1.0f), scaleVector);
    }
    std::unique_ptr<Transformation> clone() const override {
        return std::make_unique<Scaling>(*this);
    }

private:
    glm::vec3 scaleVector;
};

class DynamicTranslation : public Transformation {
public:
    DynamicTranslation(const glm::vec3& center, float& radius, float& speed)
        : center(center), radius(radius), speed(speed), angle(0.0f) {}

    glm::mat4 getMatrix() const override {
        float xOffset = radius * cos(angle);
        float zOffset = radius * sin(angle);

        return glm::translate(glm::mat4(1.0f), center + glm::vec3(xOffset, 0.0f, zOffset));
    }

    std::unique_ptr<Transformation> clone() const override {
        return std::make_unique<DynamicTranslation>(*this);
    }

    void update() {
        angle += speed;
    }

private:
    glm::vec3 center;
    float radius;
    float& speed;
    float angle; 
};

class DynamicRotation : public Transformation {
public:
    DynamicRotation(const glm::vec3 axis, float speed)
        : axis(axis), speed(speed), angle(0.0f) {}

    glm::mat4 getMatrix() const override {
        return glm::rotate(glm::mat4(1.0f), angle, axis);
    }

    std::unique_ptr<Transformation> clone() const override {
        return std::make_unique<DynamicRotation>(*this);
    }

    void update() {
        angle += speed; 
    }

private:
    glm::vec3 axis;
    float speed;
    float angle;    
};

/*
class Transformation {
public:
    Transformation() : M(glm::mat4(1.0f)) {}
    Transformation(const Transformation& other) : M(other.M) {}

    glm::mat4 getMatrix() const {
        return M;
    }

    void translate(float x, float y, float z) {
        M = glm::translate(M, glm::vec3(x, y, z));
    }

    void rotate(float angle, float x, float y, float z) {
        M = glm::rotate(M, angle, glm::vec3(x, y, z));
    }

    void scale(float s) {
        M = glm::scale(M, glm::vec3(s));
    }

    void reset() { M = glm::mat4(1.0f); }

private:
    glm::mat4 M; 
}; 
*/
#endif // !TRANSFORMATION_N