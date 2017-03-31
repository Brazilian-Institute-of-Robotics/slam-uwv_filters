#ifndef _UWV_KALMAN_FILTERS_POSE_UKF_HPP_
#define _UWV_KALMAN_FILTERS_POSE_UKF_HPP_

#include <base/Time.hpp>
#include <pose_estimation/UnscentedKalmanFilter.hpp>
#include <pose_estimation/Measurement.hpp>
#include <uwv_dynamic_model/DataTypes.hpp>
#include "PoseState.hpp"
#include "PoseUKFConfig.hpp"

namespace uwv_dynamic_model
{
    class DynamicModel;
}

namespace uwv_kalman_filters
{

class PoseUKF : public pose_estimation::UnscentedKalmanFilter<PoseState>
{
public:
    MEASUREMENT(XY_Position, 2)
    MEASUREMENT(Z_Position, 1)
    MEASUREMENT(RotationRate, 3)
    MEASUREMENT(Acceleration, 3)
    MEASUREMENT(Velocity, 3)
    MEASUREMENT(BodyEffortsMeasurement, 3)

public:
    PoseUKF(const State& initial_state, const Covariance& state_cov,
            const LocationConfiguration& location, const uwv_dynamic_model::UWVParameters& model_parameters,
            const Eigen::Vector3d& imu_in_body, double gyro_bias_tau, double acc_bias_tau);
    virtual ~PoseUKF() {}

    /* 2D Position expressed in the navigation frame */
    void integrateMeasurement(const XY_Position& xy_position);

    /* Altitude of IMU expressed in the navigation frame */
    void integrateMeasurement(const Z_Position& z_position);

    /* Rotation rates of IMU expressed in the IMU frame */
    void integrateMeasurement(const RotationRate& rotation_rate);

    /* Accelerations of IMU exxpressed in the IMU frame */
    void integrateMeasurement(const Acceleration& acceleration);

    /* Velocities expressed in the IMU frame */
    void integrateMeasurement(const Velocity& velocity);

    /* Linear efforts in the body frame */
    void integrateMeasurement(const BodyEffortsMeasurement& body_efforts);

    /* Returns rotation rate in IMU frame */
    RotationRate::Mu getRotationRate();

protected:
    void predictionStepImpl(double delta_t);


    boost::shared_ptr<uwv_dynamic_model::DynamicModel> dynamic_model;
    RotationRate::Mu rotation_rate;
    Eigen::Vector3d earth_rotation;
    Eigen::Vector3d imu_in_body;
    double gyro_bias_tau;
    double acc_bias_tau;
};

}

#endif