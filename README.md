# mr-object

`mr-object`是一个用于管理对象树的`C`库。提供一组简单易用的`API`，支持通过路径注册、查找和删除对象。

---

## 特性

- 支持基于路径的树状对象管理
- 资源占用低，适合嵌入式系统使用
- 中断安全的设计
- 简单直观的API

---

## 对象树示例：

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

## API 文档

### 根据路径查找对象

```c
struct mr_object *mr_object_find(const char *path);
```

| 参数      | 描述     |
|---------|--------|
| `path`  | 对象的路径  |
| **返回值** |        |
| 查找成功    | 对象的指针  |
| 查找失败    | `NULL` |

#### 使用示例：

```c
struct mr_object *object = mr_object_find("/devices/sensor");
if (object != NULL)
{
    /* 使用对象 */
}
```

### 注册对象到指定路径

```c
int mr_object_register(struct mr_object *object, const char *path);
```

| 参数       | 描述     |
|----------|--------|
| `object` | 要注册的对象 |
| `path`   | 对象的路径  |
| **返回值**  |        |
| 注册成功     | `0`    |
| 注册失败     | `-1`   |

#### 使用示例：

```c
#include "mr_object.h"

/* 注册对象 */
struct mr_object devices = {0};
mr_object_register(&devices, "/devices");

struct mr_object sensor = {0};
mr_object_register(&sensor, "/devices/sensor");
```

结构示意：

```c
|--- root
     |--- devices
          |--- sensor
```

### 注销对象或对象树

```c
int mr_object_unregister(struct mr_object *object);
```

| 参数       | 描述             |
|----------|----------------|
| `object` | 要注销的对象或对象树的根对象 |
| **返回值**  |                |
| 注销成功     | `0`            |
| 注销失败     | `-1`           |

注：被注销的对象下的结构将保留。

#### 使用示例：

```c
/* 注销单个对象 */
mr_object_unregister(&sensor);

/* 注销对象树（devices下的结构将保留） */
mr_object_unregister(mr_object_find("/devices"));
```

结构示意：

```c
/* 注销单个对象 */
|--- root
     |--- devices

/* 注销对象树 */
|--- root
```

### 获取对象的父对象

```c
struct mr_object *mr_object_parent_get(struct mr_object *object);
```

| 参数       | 描述        |
|----------|-----------|
| `object` | 要获取父对象的对象 |
| **返回值**  |           |
|          | 父对象的指针    |

#### 使用示例：

```c
struct mr_object *parent = mr_object_parent_get(mr_object_find("/devices/sensor"));   
```

返回示意：

```c
|--- root
     |--- devices    <-- 返回此对象
          |--- sensor
```

### 打印整个对象树或指定对象及其子对象

```c
void mr_object_tree_print(struct mr_object *object);
```

| 参数       | 描述                              |
|----------|---------------------------------|
| `parent` | 要打印的对象树的根对象，如果为 `NULL` 则打印整个对象树 |

#### 使用示例：

```c
/* 打印整个对象树 */
mr_object_tree_print(NULL);

/* 打印指定对象树 */
mr_object_tree_print(mr_object_parent_get(mr_object_find("/devices/sensor")));
```

---

## 如有任何问题，欢迎提交 issue 或 pull request
