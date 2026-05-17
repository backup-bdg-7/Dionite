// ============================================================================
// Dionite — iOS DioniteBridge: Swift ↔ C++ glue. Forwards input + lifecycle.
// ============================================================================
import Foundation

@_silgen_name("dionite_boot")     func dionite_boot()
@_silgen_name("dionite_shutdown") func dionite_shutdown()
@_silgen_name("dionite_tick")     func dionite_tick(_ dt: Float)
@_silgen_name("dionite_resize")   func dionite_resize(_ w: Int32, _ h: Int32)
@_silgen_name("dionite_set_move") func dionite_set_move(_ x: Float, _ y: Float)
@_silgen_name("dionite_set_aim")  func dionite_set_aim(_ x: Float, _ y: Float)
@_silgen_name("dionite_set_fire") func dionite_set_fire(_ b: Bool)
@_silgen_name("dionite_set_ability") func dionite_set_ability(_ idx: Int32, _ b: Bool)
@_silgen_name("dionite_set_dash") func dionite_set_dash(_ b: Bool)
@_silgen_name("dionite_pause")    func dionite_pause()
@_silgen_name("dionite_resume")   func dionite_resume()

final class DioniteBridge {
    static let shared = DioniteBridge()
    private init() {}

    func bootCore()                     { dionite_boot() }
    func shutdownCore()                 { dionite_shutdown() }
    func tick(dt: Float)                { dionite_tick(dt) }
    func resize(width: Int32, height: Int32) { dionite_resize(width, height) }
    func setMove(x: Float, y: Float)    { dionite_set_move(x, y) }
    func setAim(x: Float, y: Float)     { dionite_set_aim(x, y) }
    func setFire(_ v: Bool)             { dionite_set_fire(v) }
    func setAbility(_ idx: Int, _ v: Bool) { dionite_set_ability(Int32(idx), v) }
    func setDash(_ v: Bool)             { dionite_set_dash(v) }
    func pause()                        { dionite_pause() }
    func resume()                       { dionite_resume() }
}
