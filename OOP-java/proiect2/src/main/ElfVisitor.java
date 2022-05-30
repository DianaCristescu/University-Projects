package main;

public interface ElfVisitor {
    void addBonus(PinkElf elf, OutputChild child);
    void addBonus(BlackElf elf, OutputChild child);
    void addBonus(WhiteElf elf, OutputChild child);
    void addGift(ElfVisitable elf, Input input, OutputChild child);
    void addGift(YellowElf elf, Input input, OutputChild child);
}
