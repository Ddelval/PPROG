/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   skill.h
 * Author: jaime
 *
 * Created on 4 de octubre de 2019, 19:30
 */

#ifndef SKILL_H
#define SKILL_H

#include "types.h"
typedef enum {NOTHING = 0, STUNNER = 1, UNDOGDE = 2, POISON = 3} specialSkill;
typedef struct _skill skill;
typedef struct _entity entity;
/*
 Function name: skill_ini
 Utility: It creates a new skill structure with NULL in each camp
 Inputs:
 Outputs: skill pointer
 */
skill * skill_ini();
/*
Function name: skill_destroy
 Utility: Destroys a skill struct
 Inputs: skill to destroy (pointer)
 Outputs: skill pointer
 */
void skill_destroy(skill* skill);
/*
Function name: skill_load
 Utility: It takes a file with skill camps and inserts them into an object struct
 Inputs: skill pointer, file pointer, name of the file.
 Outputs: skill pointer
 */
skill * skill_activate(skill*skill, entity * entity);

skill * skill_getSpecial(skill *s);

#endif /* SKILL_H */
