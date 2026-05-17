// ============================================================================
// Dionite — iOS AppDelegate (Swift). Boots Metal view, hands frames to C++ core.
// ============================================================================
import UIKit
import Metal
import MetalKit

@main
class AppDelegate: UIResponder, UIApplicationDelegate {
    var window: UIWindow?

    func application(_ application: UIApplication,
                     didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        window = UIWindow(frame: UIScreen.main.bounds)
        let gameVC = GameViewController()
        window?.rootViewController = gameVC
        window?.makeKeyAndVisible()
        DioniteBridge.shared.bootCore()
        return true
    }

    func applicationWillResignActive(_ application: UIApplication) {
        DioniteBridge.shared.pause()
    }

    func applicationDidBecomeActive(_ application: UIApplication) {
        DioniteBridge.shared.resume()
    }
}
