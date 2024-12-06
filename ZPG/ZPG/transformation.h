#ifndef TRANSFORMATION_N
#define TRANSFORMATION_N

//Include GLM  
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <memory>
#include <vector>
class BasicTransform;
class Transform {
public:
    Transform() {};
    glm::mat4 getMatrix();

    //void addTransform(BasicTransform* trans);
    void addTransform(std::unique_ptr<BasicTransform> trans);

    std::vector<std::unique_ptr<BasicTransform>>transform;

};


class BasicTransform {
public:
    virtual ~BasicTransform() = default;

    virtual glm::mat4 getMatrix() const = 0;

    virtual std::unique_ptr<BasicTransform> clone() const = 0;
};
class DynamicTransformation : public BasicTransform {
public:
    virtual void update() = 0;
};

class Translation : public BasicTransform {
public:
    Translation(float x, float y, float z) : translationVector(x, y, z) {}

    glm::mat4 getMatrix() const override {
        return glm::translate(glm::mat4(1.0f), translationVector);
    }
    std::unique_ptr<BasicTransform> clone() const override {
        return std::make_unique<Translation>(*this);
    }

private:
    glm::vec3 translationVector;
};

class Rotation : public BasicTransform {
public:
    Rotation(float angle, float x, float y, float z) : angle(angle), axis(x, y, z) {}

    glm::mat4 getMatrix() const override {
        return glm::rotate(glm::mat4(1.0f), angle, axis);
    }

    std::unique_ptr<BasicTransform> clone() const override {
        return std::make_unique<Rotation>(*this);
    }

private:
    float angle;
    glm::vec3 axis;
};

class Scaling : public BasicTransform {
public:
    Scaling(float scale) : scaleVector(scale, scale, scale) {}

    glm::mat4 getMatrix() const override {
        return glm::scale(glm::mat4(1.0f), scaleVector);
    }
    std::unique_ptr<BasicTransform> clone() const override {
        return std::make_unique<Scaling>(*this);
    }

private:
    glm::vec3 scaleVector;
};

class DynamicTranslation : public DynamicTransformation {
public:
    DynamicTranslation(const glm::vec3& center, float radius, float speed)
        : center(center), radius(radius), speed(speed), angle(0.0f) {}

    glm::mat4 getMatrix() const override {
        float xOffset = radius * cos(angle);
        float zOffset = radius * sin(angle);

        return glm::translate(glm::mat4(1.0f), center + glm::vec3(xOffset, 0.0f, zOffset));
    }

    std::unique_ptr<BasicTransform> clone() const override {
        return std::make_unique<DynamicTranslation>(*this);
    }

    void update() override {
        angle += speed;
    }

private:
    glm::vec3 center;
    float radius;
    float speed;
    float angle; 
};

class DynamicRotation : public DynamicTransformation {
public:
    DynamicRotation(const glm::vec3 axis, float speed)
        : axis(axis), speed(speed), angle(0.0f) {}

    glm::mat4 getMatrix() const override {
        return glm::rotate(glm::mat4(1.0f), angle, axis);
    }

    std::unique_ptr<BasicTransform> clone() const override {
        return std::make_unique<DynamicRotation>(*this);
    }

    void update() override{
        angle += speed; 
    }

private:
    glm::vec3 axis;
    float speed;
    float angle;    
};
class BezierCurveTransform : public DynamicTransformation {
public:
    BezierCurveTransform(const glm::mat4 bezierMatrix, const glm::mat4x3 controlPoints, float speed)
        : bezierMatrix(bezierMatrix), controlPoints(controlPoints), t(0.0f), delta(speed) {}

    glm::mat4 getMatrix() const override {
        glm::vec4 parameters = glm::vec4(t * t * t, t * t, t, 1.0f);
        glm::vec3 position = parameters * bezierMatrix * glm::transpose(controlPoints);
        return glm::translate(glm::mat4(1.0f), position);
    }

    std::unique_ptr<BasicTransform> clone() const override {
        return std::make_unique<BezierCurveTransform>(*this);
    }

    void update() override {
        t += delta;

        // Clamp or reverse `t` if it goes out of bounds
        if (t > 1.0f) {
            t = 1.0f;
            delta *= -1; // Reverse direction
        }
        else if (t < 0.0f) {
            t = 0.0f;
            delta *= -1; // Reverse direction
        }
    }

private:
    glm::mat4 bezierMatrix;
    glm::mat4x3 controlPoints;
    float t;
    float delta;
};
/*
class Transformation {
public:

    glm::mat4 getModelMatrix() const {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        for (const auto& transform : transformations) {
            modelMatrix *= transform->getMatrix();
        }
        return modelMatrix;
    }
private:
    std::vector<std::unique_ptr<BasicTransform>> transformations;
};
*/


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