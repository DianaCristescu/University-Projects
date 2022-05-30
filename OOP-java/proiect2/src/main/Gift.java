package main;

public final class Gift {
    private String productName;
    private Double price;
    private String category;
    private int quantity;

    public String getProductName() {
        return productName;
    }

    public void setProductName(final String productName) {
        this.productName = productName;
    }

    public Double getPrice() {
        return price;
    }

    public void setPrice(final Double price) {
        this.price = price;
    }

    public String getCategory() {
        return category;
    }

    public void setCategory(final String category) {
        this.category = category;
    }

    public void setQuantity(final int quantity) {
        this.quantity = quantity;
    }

    public int returnQuantity() {
        return quantity;
    }
}
