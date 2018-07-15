import platform


class ColorPrinter():
    """
    Utility class used for printing colored messages.
    """

    def __init__(self, size=1):
        self.size = size
        try:
            from colorama import init
            init()
            self.HEADER = '\033[95m'
            self.OKBLUE = '\033[94m'
            self.OKGREEN = '\033[92m'
            self.WARNING = '\033[93m'
            self.FAIL = '\033[91m'
            self.ENDC = '\033[0m'
        except ImportError:
            if "windows" in platform.system().lower():
                self.HEADER = ''
                self.OKBLUE = ''
                self.OKGREEN = ''
                self.WARNING = ''
                self.FAIL = ''
                self.ENDC = ''
            else:
                self.HEADER = '\033[95m'
                self.OKBLUE = '\033[94m'
                self.OKGREEN = '\033[92m'
                self.WARNING = '\033[93m'
                self.FAIL = '\033[91m'
                self.ENDC = '\033[0m'

    def SetSize(self, size):
        self.size = size

    def InfoPrint(self, message):
        """
        Prints a purple info message.
        """
        print(self.InfoString(message))

    def InfoString(self, message):
        """
        Prints a purple info message.
        """
        return self.HEADER + "[ -INFO-]" + self.ENDC + message

    def ErrorPrint(self, message):
        """
        Prints a red error message.
        """
        print(self.FAIL + "[  ERROR] " + self.ENDC + message)

    def CompilePrint(self, percent, build, message):
        """
        Prints a compiled message, including a green percent prefix.
        """
        percent_string = "{0:.2f}".format(percent)
        if percent < 100:
            percent_string = " " + percent_string
        if percent < 10:
            percent_string = " " + percent_string
        print_string = self.OKGREEN + \
            "[" + percent_string + "%]" + self.OKBLUE + \
            "[ " + build + " ] " + self.ENDC + message
        print(print_string)

    def LinkPrint(self, build, message):
        """
        Prints a linked message, including a green link prefix.
        """
        print(self.OKGREEN + "[ LINK!!]" + self.OKBLUE +
              "[ " + build + " ] " + self.ENDC + message)

    def TestPassPrint(self, message):
        """
        Prints a test result message.
        """
        print(self.OKGREEN + "[ PASS!!]" + self.ENDC + message)

    def TestFailPrint(self, message):
        """
        Prints a test result message.
        """
        print(self.FAIL + "[ FAIL!!]" + self.ENDC + message)

    def ConfigString(self, message):
        """
        Prints a blue configure message.
        """
        return self.OKBLUE + "[ CONFIG] " + self.ENDC + message
