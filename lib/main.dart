import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';
import 'firebase_options.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(options: DefaultFirebaseOptions.currentPlatform);
  runApp(const LedApp());
}

class LedApp extends StatelessWidget {
  const LedApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Controle LED ESP8266',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.blueAccent),
        useMaterial3: true,
      ),
      home: const LedControlScreen(),
      debugShowCheckedModeBanner: false,
    );
  }
}

class LedControlScreen extends StatefulWidget {
  const LedControlScreen({super.key});

  @override
  State<LedControlScreen> createState() => _LedControlScreenState();
}

class _LedControlScreenState extends State<LedControlScreen> {
  final DatabaseReference _ref = FirebaseDatabase.instance.ref('led');
  bool _ledState = false;

  @override
  void initState() {
    super.initState();
    _listenLedState();
  }

  void _listenLedState() {
    _ref.onValue.listen((event) {
      final value = event.snapshot.value;
      setState(() {
        _ledState = value == true;
      });
    });
  }

  Future<void> _toggleLed() async {
    await _ref.set(!_ledState);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Controle do LED')),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Icon(
              _ledState ? Icons.lightbulb : Icons.lightbulb_outline,
              color: _ledState ? Colors.yellow : Colors.grey,
              size: 120,
            ),
            const SizedBox(height: 20),
            ElevatedButton(
              onPressed: _toggleLed,
              style: ElevatedButton.styleFrom(
                backgroundColor: _ledState ? Colors.red : Colors.green,
                padding: const EdgeInsets.symmetric(
                  horizontal: 40,
                  vertical: 16,
                ),
                shape: RoundedRectangleBorder(
                  borderRadius: BorderRadius.circular(30),
                ),
              ),
              child: Text(
                _ledState ? 'Desligar' : 'Ligar',
                style: const TextStyle(fontSize: 24, color: Colors.white),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
