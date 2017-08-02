// =============== STRUCTS ===============

struct coordinate {
  float x;
  float y;
  float z;
};

struct pins {
  int en;
  int stp;
  int dir;
};

// =============== FUNCTIONS ===============

float euclidean_distance(coordinate p1, coordinate p2)
{
  return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}

float round_precision(float number, float min_precision)
{
  return round(number * (1.0 / min_precision)) / (1.0 / min_precision);
}

// =============== GONDOLA CLASS ===============

class Gondola
{

coordinate current_position;

public:
  Gondola(coordinate new_position)
  {
    current_position = new_position;
    if (DEBUG)
    {
      Serial.print("Creating gondola at (");
      Serial.print(current_position.x);
      Serial.print(",");
      Serial.print(current_position.y);
      Serial.print(",");
      Serial.print(current_position.z);
      Serial.println(")");
    }
  }

  coordinate get_position()
  {
    return current_position;
  }

  void set_position(coordinate new_position)
  {
    current_position.x = new_position.x;
    current_position.y = new_position.y;
    current_position.z = new_position.z;

    if (DEBUG)
    {
      Serial.print("Set gondola position to (");
      Serial.print(current_position.x);
      Serial.print(",");
      Serial.print(current_position.y);
      Serial.print(",");
      Serial.print(current_position.z);
      Serial.println(")");
    }
  }
};

// =============== ANCHOR CLASS ===============

class Anchor
{
  coordinate anchor_position;
  pins pin;

  // state
  float spooled_distance;
  long steps_todo, steps_done, steps_goal;

public:
  Anchor(int id)
  {
    Serial.print("Creating anchor ");
    Serial.println(id);
  }

  coordinate get_position()
  {
    return anchor_position;
  }

  long missing_steps()
  {
    return steps_todo - steps_done;
  }

  void set_pins(int _enable, int _step, int _direction)
  {
    pin = {_enable, _step, _direction};
    pinMode(pin.en, OUTPUT);
    pinMode(pin.stp, OUTPUT);
    pinMode(pin.dir, OUTPUT);
    digitalWrite(pin.en, LOW);
  }

  void set_position(float _x, float _y, float _z, coordinate _gondola)
  {
    anchor_position = {_x, _y, _z};
    spooled_distance = euclidean_distance(_gondola, anchor_position);
  }

  void prepare_to_spool(coordinate new_position)
  {
    float cm_todo, cm_todo_rounded, new_spooled_distance, precision_distance;

    new_spooled_distance = euclidean_distance(anchor_position, new_position);

    cm_todo = new_spooled_distance - spooled_distance; // in cm

    if (DEBUG)
    {
      Serial.print("Spooled: ");
      Serial.print(spooled_distance);
      Serial.print("cm, Delta: ");
      Serial.print(cm_todo);
    }

    if (cm_todo < 0)
    {
      // direction_todo = 1;
      digitalWrite(pin.dir, HIGH);
    }
    else
    {
      // direction_todo = -1;
      digitalWrite(pin.dir, LOW);
    }

    spooled_distance += round_precision(cm_todo, MIN_PRECISION); // save new anchor spooled distance
    // spooled_distance += cm_todo; // save new anchor spooled distance
    cm_todo = abs(cm_todo);
    cm_todo_rounded = round_precision(cm_todo, MIN_PRECISION);
    steps_todo = (long)(cm_todo_rounded * STEP_CM); // here we do not make sure the number is not round!!!

    if (DEBUG)
    {
      Serial.print("cm, rounded to (");
      Serial.print(MIN_PRECISION);
      Serial.print("): ");
      Serial.print(cm_todo_rounded);
      Serial.print("cm, steps: ");
      Serial.print(steps_todo); // 200 steps per cm
      Serial.print(", microsteps: ");
      Serial.println(steps_todo * MICROSTEPS);
    }

    steps_todo *= MICROSTEPS; // we need to account for all microsteps
    steps_done = 0;
    steps_goal = 0;
  }

  void start_step(long start_time, float budget)
  {
    steps_goal = ceil(((float)((millis() - start_time) * steps_todo)) / budget);
    if (steps_goal > steps_todo)
    {
      steps_goal = steps_todo;
    }
    if ((steps_goal > steps_done) && (steps_done < steps_todo))
    {
      digitalWrite(pin.stp, HIGH);
    }
  }

  void end_step()
  {
    if ((steps_goal > steps_done) && (steps_done < steps_todo))
    {
      digitalWrite(pin.stp, LOW); // stop step trigger
      steps_done += 1;
    }
  }
};
