#include <Arduino.h>
#include <math.h>
#include "pico/multicore.h"

// Motor movement
#define IN1 10
#define IN2 11
#define IN3 12
#define IN4 13
#define ENA 14
#define ENB 15

// Encoder pins
#define ENCODERLEFT_PIN_A 21
#define ENCODERLEFT_PIN_B 22
#define ENCODERRIGHT_PIN_A 8
#define ENCODERRIGHT_PIN_B 9


// Timer variables to avoid using the function delay()
typedef struct{
  uint32_t curr_time = 0;             // Current time since beginning of the execution
  uint32_t loop_interval = 100;       // Interval between each loop iteration
  uint32_t last_loop_time = 0;        // Time of the last loop iteration
} TIME_CTRL;

// Robot position and orientation
typedef struct {
  float x=0, y=0, theta=0;            // Position in x, y and orientation according wall
} POSE;

// Motor control
typedef struct {
  float vel_left, vel_right;          // Current velocity for both motors
  float pwm_offset=50;                // Minimum pwm to avoid motors not moving
  float pwm_left, pwm_right;          // PWM values for both motors     
  float vel_2_pwm=255/100;            // Conversion factor from velocity to PWM in percentage
  float slip_compensation=1;          // How much the right motor is faster than the left motor
} MOTORS;


// Tick counter for encoders
volatile int encoderLeftCount = 0;  
volatile int encoderRightCount = 0;

// Create global instance of the classes
POSE robot_pose;
MOTORS motors;
TIME_CTRL time_ctrl;


/**
 * Converts velocity to PWM values for both motors.
 * PWM value is calculated based on the velocity, slip compensation, and velocity-to-PWM conversion factor.
 * The calculated PWM value is constrained between the PWM offset and 255.
 */
void vel_to_pwm() {
  if (motors.vel_left == 0)
    motors.pwm_left = 0;
  else
    motors.pwm_left = constrain(motors.pwm_offset + (2-motors.slip_compensation) * abs(motors.vel_left) * motors.vel_2_pwm,motors.pwm_offset,255);

  if (motors.vel_right == 0)
    motors.pwm_right = 0;
  else
    motors.pwm_right = constrain(motors.pwm_offset + motors.slip_compensation * abs(motors.vel_right) * motors.vel_2_pwm,motors.pwm_offset,255);

}


/**
 * Resets the motor values to zero.
 * 
 * @param motors The MOTORS struct containing the motor values.
 */
void reset_motors(MOTORS& motors) {
  motors.vel_left = 0;
  motors.vel_right = 0;
  motors.pwm_left = 0;
  motors.pwm_right = 0;
}

/**
 * Initializes the motors.
 * Sets the initial values for velocity, PWM, slip compensation, and velocity to PWM conversion.
 * Configures the pin modes for motor control.
 */
void init_motors() {
  motors.vel_left = 0;
  motors.vel_right = 0;
  motors.pwm_left = 0;
  motors.pwm_right = 0;
  motors.slip_compensation = 1.10;  // Right / Left
  motors.vel_2_pwm = 255/100;

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}


/**
 * Counts the ticks of the motors
 * The following functions deal with the counting of the encoders' ticks.
 * They run on a different core to avoid missing ticks.
 */
  void encoderRightISR() {
    if (digitalRead(ENCODERRIGHT_PIN_B)) {
      encoderRightCount--;
    } else {
      encoderRightCount++;
    }
  }

  void encoderLeftISR() { // Define the encoderLeftISR function
    if (digitalRead(ENCODERLEFT_PIN_B)) {
      encoderLeftCount++;
    } else {
      encoderLeftCount--;
    }
  }

  void encoderISR() {
    while (1) {
      encoderRightISR();
      encoderLeftISR();
    }
  }

  void reset_encoders(){
    encoderRightCount = 0;
    encoderLeftCount = 0;
  }


/**
 * Initializes the encoders.
 * This function launches the `encoderISR` function on core 1.
 */
void init_encoders() {
  multicore_launch_core1(encoderISR);
}

/**
 * Initializes the robot's pose.
 * Sets the initial values for the robot's position and orientation.
 */
void init_pose() {
  robot_pose.x = 0;
  robot_pose.y = 0;
  robot_pose.theta = 0;
}

/**
 * @brief Initializes the Raspberry Pi Pico W board and sets up the initial configuration.
 * 
 * This function is called only once when the Arduino board is powered on or reset.
 * It initializes the serial communication, motor drivers, encoders, and pose.
 * It also resets the encoder values to zero.
 */
void setup() {
  Serial.begin(115200);

  init_motors();
  init_encoders();
  init_pose();

  reset_encoders();
}


/**
 * Sets the outputs for controlling the motors based on the current velocity values.
 * This function determines the direction (rotating front or backwards) of each motor and sets the corresponding input pins accordingly.
 * It also sets the PWM values for each motor to control their speed.
 */
void set_outputs() {
  vel_to_pwm();
  if (motors.vel_left > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }

  if (motors.vel_right > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  
  analogWrite(ENA, motors.pwm_left);
  analogWrite(ENB, motors.pwm_right);
}



/**
 * The main loop function of the program.
 * This function is called repeatedly in an infinite loop.
 * It updates the current time, checks if the loop interval has passed,
 * sets the motor velocities, updates the outputs, and resets the encoders.
 */
void loop() 
{
    // Regarding timers, function delay() should be avoided
    time_ctrl.curr_time = millis();
    if (time_ctrl.curr_time - time_ctrl.last_loop_time >= time_ctrl.loop_interval) {
      time_ctrl.last_loop_time = time_ctrl.curr_time;
      // Define the robot's movement
      motors.vel_left=100;
      motors.vel_right=100;
      // Set the outputs
      set_outputs();
      // Reset counters
      reset_encoders();
    }
}




