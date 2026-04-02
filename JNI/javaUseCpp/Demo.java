class Demo {
    public static void main(String[] args) {
        String libPath = "/Users/junjiecui/workspace/cpp_notebook/JNI/javaUseCpp/jniDemo.so";
        System.load(libPath);
        System.out.println("Hello, Java!"); 
        System.out.println("3 + 4 = " + add(3, 4));
    }
    public static native int add(int a, int b);
}