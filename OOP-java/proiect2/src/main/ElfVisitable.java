package main;

public interface ElfVisitable {
    void addBonus(ElfVisitor visitor, OutputChild child);
    void addGift(ElfVisitor visitor, Input input, OutputChild child);
}
