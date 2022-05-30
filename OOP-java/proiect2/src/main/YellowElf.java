package main;

public final class YellowElf extends WhiteElf {
    @Override
    public void addGift(final ElfVisitor visitor, final Input input, final OutputChild child) {
        visitor.addGift(this, input, child);
    }
}
