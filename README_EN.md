# mr-object

`mr-object` is a C library for managing object trees.
It provides a set of simple and easy-to-use APIs for registering, finding, and removing objects via paths.

---

## Features

- Supports path-based hierarchical object management
- Low resource usage, suitable for embedded systems
- Interrupt-safe design
- Simple and intuitive API

---

## Object Tree Example:

```c
|--- root
     |--- devices
          |--- sensors
               |--- sensor1
               |--- sensor2
               |--- sensor3
               |--- sensor4
          |--- controls
               |--- temperature
                    |--- control_a
                    |--- control_b
                    |--- control_c
               |--- humidity
                    |--- control_x
                    |--- control_y
                    |--- control_z
          |--- actuators
               |--- motor1
               |--- motor2
               |--- valve_a
               |--- valve_b
     |--- network
          |--- ethernet
               |--- switch_a
               |--- switch_b
          |--- wifi
               |--- access_point_1
               |--- access_point_2
     |--- power
          |--- supply_a
          |--- supply_b
          |--- backup_generator
     |--- analytics
          |--- dashboard
          |--- reporting
          |--- prediction
     |--- settings
          |--- user_management
          |--- system_config
          |--- notifications
```

---

## API Documentation

### Find an Object by Path

```c
struct mr_object *mr_object_find(const char *path);
```

| Parameter        | Description            |
|------------------|------------------------|
| `path`           | The path of the object |
| **Return Value** |                        |
| Success          | Pointer to the object  |
| Failure          | `NULL`                 |

#### Example Usage:

```c
struct mr_object *object = mr_object_find("/devices/sensor");
if (object != NULL)
{
    /* Use the object */
}
```

### Register an Object to a Specified Path

```c
int mr_object_register(struct mr_object *object, const char *path);
```

| Parameter        | Description                 |
|------------------|-----------------------------|
| `object`         | The object to be registered |
| `path`           | The path of the object      |
| **Return Value** |                             |
| Success          | `0`                         |
| Failure          | `-1`                        |

#### Example Usage:

```c
#include "mr_object.h"

/* Register an object */
struct mr_object devices = {0};
mr_object_register(&devices, "/devices");

struct mr_object sensor = {0};
mr_object_register(&sensor, "/devices/sensor");
```

The resulting structure:

```c
|--- root
     |--- devices
          |--- sensor
```

### Unregister an Object or an Object Tree

```c
int mr_object_unregister(struct mr_object *object);
```

| Parameter        | Description                                                         |
|------------------|---------------------------------------------------------------------|
| `object`         | The object or the root object of the object tree to be unregistered |
| **Return Value** |                                                                     |
| Success          | `0`                                                                 |
| Failure          | `-1`                                                                |

Note: The structure under the unregistered object will be preserved.

#### Example Usage:

```c
/* Unregister a single object */
mr_object_unregister(&sensor);

/* Unregister an object tree (the structure under /devices will be preserved) */
mr_object_unregister(mr_object_find("/devices"));
```

The resulting structure:

```c
/* Unregister a single object */
|--- root
     |--- devices

/* Unregister an object tree */
|--- root
```

### Get the Parent Object of an Object

```c
struct mr_object *mr_object_parent_get(struct mr_object *object);
```

| Parameter         | Description                                       |
|-------------------|---------------------------------------------------|
| `object`          | The object whose parent object is to be retrieved |
| **Return Value**  |                                                   |
| The parent object |

#### Example Usage:

```c
struct mr_object *parent = mr_object_parent_get(mr_object_find("/devices/sensor"));   
```

The result:

```c
|--- root
     |--- devices    <-- This object is returned
          |--- sensor
```

### Print the Entire Object Tree or a Specified Object and Its Subobjects

```c
void mr_object_tree_print(struct mr_object *object);
```

| Parameter | Description                                                                                          |
|-----------|------------------------------------------------------------------------------------------------------|
| `parent`  | The root object of the object tree to be printed. If `NULL`, the entire object tree will be printed. |

#### Example Usage:

```c
/* Print the entire object tree */
mr_object_tree_print(NULL);

/* Print a specific object tree */
mr_object_tree_print(mr_object_parent_get(mr_object_find("/devices/sensor")));
```

---

## If you have any questions, feel free to submit an issue or a pull request