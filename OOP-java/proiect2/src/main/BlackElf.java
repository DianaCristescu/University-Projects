package main;

public final class BlackElf implements ElfVisitable {
    @Override
    public void addBonus(final ElfVisitor visitor, final OutputChild child) {
        visitor.addBonus(this, child);
    }

    @Override
    public void addGift(final ElfVisitor visitor, final Input input, final OutputChild child) {
        visitor.addGift(this, input, child);
    }
}
