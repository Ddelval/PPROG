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
typedef struct _skill skill;
typedef struct _entity entity;

skill * skill_ini();

void skill_destroy(skill* skill);

skill * skill_activate(skill*skill, entity * entity);


#endif /* SKILL_H */

