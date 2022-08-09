#include <iostream>
#include <stdexcept>
#include <exception>
#include <functional>
struct SpeedUpdate
{
    double Velocity;
};

struct Car_Detected
{
    double Distance_m;
    double Velocity;
};

struct BrakeCommand
{
    double time_to_collision_s;
};

struct ServiceBus
{
    void Publish(const BrakeCommand&);
};
template<typename T>
struct AutoBrake
{
    AutoBrake(const T& publish) : publish{publish}, speed{}
    {

    }

    void observe(const SpeedUpdate&){}
    void observe(const Car_Detected&){}
    void set_collision_threshold (double x)
    {
        collision_threshold = x;
    }
    auto get_collision_threshold()
    {
        return collision_threshold;
    }

    double get_speed() const
    {
        return speed;
    }


private:
    double collision_threshold;
    double speed;
    const T& publish;


};

constexpr void assert_that(bool statement, const char* message)
{
    if(!statement) throw std::runtime_error(message);
}

void inititial_speed_is_zero()
{
    AutoBrake autoBrake{[](const BrakeCommand&){}};
    assert_that(autoBrake.get_speed() == 0L, "speed is not zero");
}

void rut_unit_test(void(*unitTest)(), const char* name)
{
    try
    {
        unitTest();
        printf("[+] %s:\t Test successful\n", name);
    }
    catch (const std::exception& e)
    {
        printf("[-] %s:\t Test failure:\nError: %s\n", name, e.what());
    }
}

//void rut_unit_test(std::function<void()> unittest, const char* name)
//{
//
//}

int main() {
//    ServiceBus bus{};
//    AutoBrake autoBrake(
//            [bus] (const auto& cmd) {
//                bus.Publish(cmd);
//            }
//            );
//    while (true)
//    {
//        autoBrake.observe(SpeedUpdate{10L});
//        autoBrake.observe(Car_Detected{250L, 25L});
//
//    }

    rut_unit_test(inititial_speed_is_zero, "initial speed is 0");
}

