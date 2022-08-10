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
    AutoBrake(const T& publish) : publish{publish}, speed{}, collision_threshold{5L}
    {

    }

    void observe(const SpeedUpdate& x)
    {
        speed = x.Velocity;
    }
    void observe(const Car_Detected&){}
    void set_collision_threshold (double x)
    {
        if (x < 1)
            throw std::runtime_error{"Collision less then 1"};
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

void initial_sensetivity_is_five()
{
    AutoBrake autoBrake{[](BrakeCommand&){}};
    assert_that(autoBrake.get_collision_threshold() == 5L, "sensitivity is not 5");
}

void inititial_speed_is_zero()
{
    AutoBrake autoBrake{[](const BrakeCommand&){}};
    assert_that(autoBrake.get_speed() == 0L, "speed is not zero");
}

void sensitivity_grater_than_1()
{
    AutoBrake autoBrake{[](){}};
    try
    {
        autoBrake.set_collision_threshold(0.5L);
    }
    catch (const std::exception&)
    {
        return;
    }
    assert_that(false, "no exception thrown");
}

void speed_is_saved()
{
    AutoBrake autoBrake{[](){}};
    autoBrake.observe(SpeedUpdate{100L});
    assert_that(100L == autoBrake.get_speed(), "speed not saved to 100");
    autoBrake.observe(SpeedUpdate{50L});
    assert_that(50L == autoBrake.get_speed(), "speed not saved to 50");
    autoBrake.observe(SpeedUpdate{0L});
    assert_that(0L == autoBrake.get_speed(), "speed not saved to 0");

}

void run_unit_test(void(*unitTest)(), const char* name)
{
    try
    {
        unitTest();
        printf("[+] %s:\t Test successful\n", name);
    }
    catch (const std::exception& e)
    {
        printf("[-] %s:\t Test failure: %s", name, e.what());
    }
}

//void run_unit_test(std::function<void()> unittest, const char* name)
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

    run_unit_test(inititial_speed_is_zero, "initial speed is 0");
    run_unit_test(initial_sensetivity_is_five, "initial sensitivity is 5");
    run_unit_test(sensitivity_grater_than_1, "Sensitivity grater than 1");
    run_unit_test(speed_is_saved, "Speed is saved");
}

