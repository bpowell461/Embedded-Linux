# Define the compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Define any directories containing header files other than /usr/include
#
INCLUDES = 

# Define library paths in addition to /usr/lib
#
LFLAGS = 

# Define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
#
LIBS = -lpthread

# Define the C source files
SRCS = syslog.c pthread.c

# Define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#   For each word in 'name' replace 'string1' with 'string2'
#
OBJS = $(SRCS:.c=.o)

# Define the executable file 
MAIN = example

#
# The following part of the Makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file.
#

all:    $(MAIN)
    @echo  Compiling complete

$(MAIN): $(OBJS) 
    $(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# This is a suffix replacement rule for building .o's from .c's
# It uses automatic variables $<: the name of the prerequisite of 
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
.c.o:
    $(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
    $(RM) *.o *~ $(MAIN)

depend: $(SRCS)
    makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it