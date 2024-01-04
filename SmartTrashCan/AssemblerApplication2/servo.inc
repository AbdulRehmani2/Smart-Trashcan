/*
 * IncFile1.inc
 *
 *  Created: 02/01/2024 2:39:31 pm
 *   Author: AbdulRehman
 */ 
 .macro STSw
cli
STS @0, @1
STS @0-1, @2
sei
.endmacro
.macro LDSw
cli
LDS @1, @2-1
LDS @0, @2
sei
.endmacro