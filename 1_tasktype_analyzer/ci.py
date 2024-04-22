class ContinueI(gdb.Command):
    """
Continue until instruction with given opcode.

    ci OPCODE

Example:

    ci callq
    ci mov
"""
    def __init__(self):
        super().__init__(
            'ci',
            gdb.COMMAND_BREAKPOINTS,
            gdb.COMPLETE_NONE,
            False
        )
    def invoke(self, arg, from_tty):
        nfound = 0
        ENDADDR = 1068458796


        thread = gdb.inferiors()[0].threads()[0]
        while thread.is_valid():
            try:
                gdb.execute('si', to_string=True)
                frame = gdb.selected_frame()
                arch = frame.architecture()
                pc = gdb.selected_frame().pc()
                #gdb.write(str(pc) + '\n')
                instruction = arch.disassemble(pc)[0]['asm']
                #gdb.write(instruction + '\n')
                #if(pc < ENDADDR):
                with open("instructionseq.txt", "a") as myfile:
                    myfile.write(instruction + '\n')
            except:
                gdb.write('Error \n')
                pass

ContinueI()
