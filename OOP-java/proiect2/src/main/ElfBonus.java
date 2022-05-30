package main;

public final class ElfBonus implements ElfVisitor {

    @Override
    public void addBonus(final PinkElf elf, final OutputChild child) {
        final int percentage1 = 30;
        final int percentage2 = 100;
        Double budget = child.getAssignedBudget();
        budget += budget * percentage1 / percentage2;
        child.setAssignedBudget(budget);
    }

    @Override
    public void addBonus(final BlackElf elf, final OutputChild child) {
        final int percentage1 = 30;
        final int percentage2 = 100;
        Double budget = child.getAssignedBudget();
        budget -= budget * percentage1 / percentage2;
        child.setAssignedBudget(budget);
    }

    @Override
    public void addBonus(final WhiteElf elf, final OutputChild child) {
        return;
    }

    @Override
    public void addGift(final ElfVisitable elf, final Input input, final OutputChild child) {
        return;
    }

    @Override
    public void addGift(final YellowElf elf, final Input input, final OutputChild child) {
        if (child.getReceivedGifts() == null || child.getReceivedGifts().size() == 0) {
            String category = child.getGiftsPreferences().get(0);
            Gift chosenGift = null;

            for (Gift gift : input.getInitialData().getSantaGiftsList()) {
                if (gift.getCategory().equals(category)) {
                    if (chosenGift == null
                            || Double.compare(gift.getPrice(), chosenGift.getPrice()) < 0) {
                        chosenGift = gift;
                    }
                }
            }

            if (chosenGift != null && chosenGift.returnQuantity() > 0) {
                chosenGift.setQuantity(chosenGift.returnQuantity() - 1);
                child.getReceivedGifts().add(chosenGift);
            }
        }
    }
}
