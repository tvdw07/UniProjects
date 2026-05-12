from ultralytics import YOLO

def main():
    model_name = "yolov8x.pt"
    print(f"Exporting {model_name} to TensorRT engine format...")
    try:
        model = YOLO(model_name)
        # Export to TensorRT engine
        model.export(format='engine', device='0', half=True)
        print("Export complete.")
    except Exception as e:
        print(f"Error during export: {e}")

if __name__ == "__main__":
    main()

