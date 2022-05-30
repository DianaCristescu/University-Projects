package main;

public final class ElfFactory {
    public ElfVisitable getElf(final String elfType) {
        if (elfType.equals("pink")) {
            return new PinkElf();
        } else if (elfType.equals("black")) {
            return new BlackElf();
        } else if (elfType.equals("yellow")) {
            return new YellowElf();
        } else {
            return new WhiteElf();
        }
    }
}
