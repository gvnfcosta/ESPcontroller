import 'package:firebase_core/firebase_core.dart' show FirebaseOptions;
import 'package:flutter/foundation.dart'
    show defaultTargetPlatform, TargetPlatform;

class DefaultFirebaseOptions {
  static FirebaseOptions get currentPlatform {
    return const FirebaseOptions(
      apiKey: "AIzaSyC6OeUnmrRX1_pCm-YQVhPJdt6KmVVIH0o",
      authDomain: "espcontroller-a8a69.firebaseapp.com",
      databaseURL:
          "https://espcontroller-a8a69-default-rtdb.europe-west1.firebasedatabase.app",
      projectId: "espcontroller-a8a69",
      storageBucket: "espcontroller-a8a69.firebasestorage.app",
      messagingSenderId: "571493867295",
      appId: "1:571493867295:web:b5fa5305b7f5b6c2ee9a2a",
    );
  }
}
