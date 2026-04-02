class Demo {
    static {
        String library_path = "/Users/junjiecui/workspace/cpp_notebook/JNI/cppUseJava/jniDemo.so";
        System.load(library_path);
    }

    public int a;
    public static int b;

    public Demo() {
        a = 1;
        b = 2;
    }

    public void show() {
        System.out.println("show");
    }

    public static native void showInCpp();
    public native void showInCpp2();

    public static void main(String[] args) {
        Demo demo = new Demo();
        demo.show();
        demo.showInCpp();
        demo.showInCpp2();
    }
}