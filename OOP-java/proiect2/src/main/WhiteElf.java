package main;

public class WhiteElf implements ElfVisitable {
    @Override
    public final void addBonus(final ElfVisitor visitor, final OutputChild child) {
        visitor.addBonus(this, child);
    }

    @Override
    public void addGift(final ElfVisitor visitor, final Input input, final OutputChild child) {
        visitor.addGift(this, input, child);
    }
}
