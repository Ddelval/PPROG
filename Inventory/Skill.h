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
typedef enum {STUNNER = 0, UNDODGE = 1, NORMAL = 2} specialSkill;
typedef struct _skill Skill;

/*
     Function name: skill_ini
     Utility: It creates a new skill structure with NULL or -1 in each field
     Inputs:
     Outputs: skill pointer
*/
Skill * skill_ini();
/*
     Function name: skill_load
     Utility: Loads a skill from a given file
     Inputs: FILE *f: previously opened file
     Outputs: skill pointer with the loaded skill
     The file format is:
        ID
        NAME
        DESCRIPTION
        SPECIAL_SKILL
        ATTRIBUTES (SELF)
        ATTRIBUTES (ATTACK)
*/
Skill* skill_load(FILE* f);


/*
     Function name: skill_copy
     Utility: Creates a new skill with the same information as the given skill
     Inputs: Skill* sk: skill to be copied (pointer)
     Outputs: A new skill with the same information as the one passed as input
*/
Skill* skill_copy(Skill* sk);
/*
     Function name: skill_getId
     Utility: Gets the skill's id.
     Inputs: Skill* s: skill to read the id
     Outputs: An integer representing the skill's id or -1 in case of error.
*/
int skill_getId(Skill *s);
/*
     Function name: skill_getName
     Utility: Gets the skill's name.
     Inputs: Skill* s: skill to read
     Outputs: A string containing the skill's name or NULL in case of error.
*/
char * skill_getName(Skill *s);
/*
     Function name: skill_getDescription
     Utility: Gets the skill's description.
     Inputs: Skill* s: skill to read
     Outputs: A string containing the skill's description or NULL in case of error.
*/
char * skill_getDesc(Skill *s);
/*
     Function name: skill_getSpecial
     Utility: Gets the skill's special skill.
     Inputs: Skill* s: skill to read
     Outputs: An integer representing the special skill of the given skill or -1 in case of error.
*/
int skill_getSpecial(Skill *s);
/*
     Function name: skill_free
     Utility: Destroys a skill struct
     Inputs: Skill* skill: skill to destroy (pointer)
     Outputs:
*/
void skill_free(Skill * s);
/*
     Function name: skill_getAtbself
     Utility: Returns a copy of the self-attributes
     Inputs: Skill* skill: skill to be used
     Outputs: An Attributes* with a copy of the self-attributes
*/
Attributes * skill_getAtbself(Skill *s);
/*
     Function name: skill_getAtbatk
     Utility: Returns a copy of the attacking-attributes
     Inputs: Skill* skill: skill to be used
     Outputs: An Attributes* with a copy of the attacking-attributes
*/
Attributes * skill_getAtbatk(Skill *s);
/*
     Function name: skill_readFromFile
     Utility: Loads a skill with a certain id from a given file
     Inputs: FILE *f: previously opened file
             int id: The id of the skill we are looking for
     Outputs: skill pointer with the loaded skill
     The file format is:
        ID
        NAME
        DESCRIPTION
        SPECIAL_SKILL
        ATTRIBUTES (SELF)
        ATTRIBUTES (ATTACK)
*/
Skill * skill_readFromFile(char *file, int id);

#endif /* SKILL_H */
