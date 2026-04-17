package com.example.carcontroller

import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import com.example.carcontroller.ui.theme.CarControllerTheme
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.widget.Button
import java.io.OutputStream
import java.util.*
import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.unit.sp
import androidx.compose.ui.unit.dp
import androidx.compose.ui.graphics.Color
import android.content.pm.PackageManager

class MainActivity : ComponentActivity() {
    private var socket: BluetoothSocket? = null
    private var output: OutputStream? = null

    var isConnecting = false

    private val DEVICE_MAC = "B8:27:EB:BF:16:1E"
    private val UUID_SPP = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")

    private fun connectBluetooth() {
        Thread {
            val adapter = BluetoothAdapter.getDefaultAdapter()
		Log.e("BT", "Bonded devices:")
		adapter.bondedDevices.forEach {
    			Log.e("BT", " - ${it.name} / ${it.address}")
		}

            if (adapter == null || !adapter.isEnabled) {
                Log.e("BT", "Bluetooth not ready")
                return@Thread
            }

            val device = adapter.bondedDevices.find {
                it.address == DEVICE_MAC.trim().uppercase()
            }

            if (device == null) {
                Log.e("BT", "Device not found")
                return@Thread
            }

            try {
                adapter.cancelDiscovery()

		val method = device.javaClass.getMethod(
        	    "createRfcommSocket",
                    Int::class.javaPrimitiveType
                )
		socket = method.invoke(device, 1) as BluetoothSocket

                Log.e("BT", "Connecting...")
                socket?.connect()

                output = socket?.outputStream
                Log.d("BT", "Connected!")

            } catch (e: Exception) {
                Log.e("BT", "Connection failed", e)
            }
        }.start()
    }
    private fun send(cmd: String) {
        try {
            if (output == null) {
                Log.e("BT", "Output stream is NULL")
                return
            }

            Log.d("BT", "Sending: $cmd")

            output!!.write((cmd + "\n").toByteArray())
            output!!.flush()

        } catch (e: Exception) {
            Log.e("BT", "Send failed", e)
        }
    }
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        if (checkSelfPermission(android.Manifest.permission.BLUETOOTH_CONNECT)
            != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(
                arrayOf(android.Manifest.permission.BLUETOOTH_CONNECT), 1)
        }

        setContent {
            CarControllerTheme {
                Column(
                    modifier = Modifier.fillMaxSize(),
                    verticalArrangement = Arrangement.Center,
                    horizontalAlignment = Alignment.CenterHorizontally
                ) {
                    Text(
                        text = "Bluetooth Car Controller App",
                        style = MaterialTheme.typography.bodyMedium
                    )
                    Text(
                        text = "C. Villani",
                        fontSize = 12.sp,
                        color = Color.Gray
                    )

                    Spacer(modifier = Modifier.height(20.dp))

                    Button(onClick = { send("F\r\n") }) {
                        Text("F")
                    }

                    Row {
                        Button(onClick = { send("L\r\n") }) { Text("L") }
                        Button(onClick = { send("S\r\n") }) { Text("S") }
                        Button(onClick = { send("R\r\n") }) { Text("R") }
                    }

                    Button(onClick = { send("B\r\n") }) {
                        Text("B")
                    }
                    Button(onClick = {
                       connectBluetooth()
                    }) {
                        Text("Connect")
                    }
                }
            }
        }
    }
}
@Composable
fun Greeting(name: String, modifier: Modifier = Modifier) {
    Text(
        text = "Hello $name!",
        modifier = modifier
    )
}

@Preview(showBackground = true)
@Composable
fun GreetingPreview() {
    CarControllerTheme {
        Greeting("Android")
    }
}

