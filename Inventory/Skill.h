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


#include "Attributes.h"
typedef enum {STUNNER = 0, UNDOGDE = 1, POISON = 2} specialSkill;
typedef struct _skill Skill;
//typedef struct _entity entity;
/*
 Function name: skill_ini
 Utility: It creates a new skill structure with NULL in each camp
 Inputs:
 Outputs: skill pointer
 */
Skill * skill_ini();
/*
Function name: skill_destroy
 Utility: Destroys a skill struct
 Inputs: skill to destroy (pointer)
 Outputs: skill pointer
 */
void skill_destroy(Skill* skill);
/*
Function name: skill_load
 Utility: It takes a file with skill camps and inserts them into an object struct
 Inputs: skill pointer, file pointer, name of the file.
 Outputs: skill pointer
 */
//skill * skill_activate(skill*skill, entity * entity);

int skill_getId(Skill *s);

char * skill_getName(Skill *s);

char * skill_getDesc(Skill *s);

int skill_getSpecial(Skill *s);

void skill_free(Skill * s);

Attributes * skill_getAtbself(Skill *s);

Attributes * skill_getAtbatk(Skill *s);

Skill * skill_readFromFile(char *file, int id);

#endif /* SKILL_H */
