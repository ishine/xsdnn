//
// Created by Andrei R. on 28.12.22.
// Copyright (c) 2022 xsDNN. All rights reserved.
//

#ifndef XSDNN_UTILS_H
#define XSDNN_UTILS_H

#define DNN_UNUSED_PARAMETER(x) (void)(x)

namespace xsdnn {

template <typename T>
class index3d {
public:
    index3d(T channel, T width, T height) : channel_(channel), width_(width), height_(height) {}
    index3d() : channel_(0), width_(0), height_(0) {}

    std::array<Eigen::DenseIndex, 3> shape() const {
        using DI = Eigen::DenseIndex;
        std::array<Eigen::DenseIndex, 3> dim = {static_cast<DI>(channel_),
                                                static_cast<DI>(width_),
                                                static_cast<DI>(height_)};
        return dim;
    }

    T area() const {
        return width_ * height_;
    }

    T size() const {
        return channel_ * width_ * height_;
    }

private:
    T channel_;
    T width_;
    T height_;
};

using shape3d = index3d<Eigen::DenseIndex>;

enum class tensor_type : int32_t {
    // input/output data
    data = 0x0001000,  // input/output data

    // trainable parameters
    weight = 0x0002000,
    bias   = 0x0002001,

    label = 0x0004000,
};

bool is_trainable_concept(tensor_type type_) {
    bool value;
    switch (type_) {
        case tensor_type::data:
            value = false;
            break;
        case tensor_type::label:
            value = false;
            break;
        case tensor_type::weight:
            value = true;
            break;
        case tensor_type::bias:
            value = true;
            break;
        default:
            throw xs_error("Unsupported tensor type");
    }
    return value;
}

void tensor_fill(Scalar* data, size_t size, Scalar value) {
    for (size_t i = 0; i < size; ++i) {
        data[i] = value;
    }
}

Matrix transpose(Matrix& m) {
    auto shuffle_idx = std::array<Eigen::DenseIndex, 2> {1, 0};
    return m.shuffle(shuffle_idx);
}


Tensor_4D _3D_to_4D(std::array<Eigen::DenseIndex, 3> copies, /*batch_size, height, width*/
                    std::array<Eigen::DenseIndex, 4> shape,  /*batch_size, channel, height, width*/
                    Tensor_3D& t_) {
    return t_.broadcast(copies).reshape(shape);
}

/*
 * This method can transform 4D Tensor to 2D ColMajor Matrix if shuffle & shape param correct
 *
 * >>> Tranform 4D(60, 1, 28, 1) -> 2D(28, 60)
 * >>> shuffle_ = {2, 0, 1, 3}
 * >>> shape_   = {28, 60}
 */
Matrix _4D_to_2D(std::array<Eigen::DenseIndex, 4> shuffle_,
                 std::array<Eigen::DenseIndex, 2> shape_,
                 Tensor_4D& t_) {
    return t_.shuffle(shuffle_).reshape(shape_);
}

} // xsdnn



#endif //XSDNN_UTILS_H
