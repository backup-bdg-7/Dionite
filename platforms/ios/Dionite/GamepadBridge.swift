// ============================================================================
// Dionite — iOS GamepadBridge: GameController framework → C++ core
// Supports Xbox / PlayStation / MFi controllers via GCController.
// ============================================================================
import Foundation
import GameController

final class GamepadBridge {
    static let shared = GamepadBridge()
    private init() {}

    private var controller: GCController?

    func start() {
        NotificationCenter.default.addObserver(self, selector: #selector(controllerConnected), name: .GCControllerDidConnect, object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(controllerDisconnected), name: .GCControllerDidDisconnect, object: nil)
        GCController.startWirelessControllerDiscovery {}
        if let c = GCController.controllers().first { attach(c) }
    }

    @objc private func controllerConnected(_ note: Notification)    { if let c = note.object as? GCController { attach(c) } }
    @objc private func controllerDisconnected(_ note: Notification) { controller = nil }

    private func attach(_ c: GCController) {
        controller = c
        c.extendedGamepad?.buttonA.valueChangedHandler = { _, _, pressed in DioniteBridge.shared.setFire(pressed) }
        c.extendedGamepad?.buttonB.valueChangedHandler = { _, _, pressed in DioniteBridge.shared.setDash(pressed) }
        c.extendedGamepad?.buttonX.valueChangedHandler = { _, _, pressed in DioniteBridge.shared.setAbility(0, pressed) }
        c.extendedGamepad?.buttonY.valueChangedHandler = { _, _, pressed in DioniteBridge.shared.setAbility(1, pressed) }
    }

    func pumpToCore() {
        guard let gp = controller?.extendedGamepad else { return }
        let lx = gp.leftThumbstick.xAxis.value
        let ly = -gp.leftThumbstick.yAxis.value
        let rx = gp.rightThumbstick.xAxis.value
        let ry = -gp.rightThumbstick.yAxis.value
        DioniteBridge.shared.setMove(x: lx, y: ly)
        DioniteBridge.shared.setAim(x: rx, y: ry)
        if gp.rightTrigger.isPressed { DioniteBridge.shared.setFire(true) }
        else if gp.buttonA.value == 0 && !gp.rightTrigger.isPressed { DioniteBridge.shared.setFire(false) }
    }
}
