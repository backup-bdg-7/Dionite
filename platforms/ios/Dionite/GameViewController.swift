// ============================================================================
// Dionite — iOS GameViewController: hosts MTKView and dispatches input
// ============================================================================
import UIKit
import MetalKit

class GameViewController: UIViewController, MTKViewDelegate {
    var metalView: MTKView!
    var device: MTLDevice!
    var commandQueue: MTLCommandQueue!
    var pipelineState: MTLRenderPipelineState!
    var leftStick = VirtualJoystick(frame: CGRect(x: 40, y: 0, width: 160, height: 160))
    var rightStick = VirtualJoystick(frame: CGRect(x: 0, y: 0, width: 160, height: 160))
    var fireButton = UIButton(type: .system)

    private var lastFrameTime: CFTimeInterval = 0

    override func viewDidLoad() {
        super.viewDidLoad()
        device = MTLCreateSystemDefaultDevice()
        metalView = MTKView(frame: view.bounds, device: device)
        metalView.colorPixelFormat = .bgra8Unorm
        metalView.depthStencilPixelFormat = .depth32Float
        metalView.preferredFramesPerSecond = 120
        metalView.delegate = self
        metalView.autoresizingMask = [.flexibleWidth, .flexibleHeight]
        view.addSubview(metalView)

        commandQueue = device.makeCommandQueue()
        buildPipeline()
        setupTouchControls()

        // Auto-detect gamepad attach
        GamepadBridge.shared.start()

        lastFrameTime = CACurrentMediaTime()
    }

    func setupTouchControls() {
        leftStick.frame.origin.y = view.bounds.height - 200
        leftStick.onChange = { dx, dy in DioniteBridge.shared.setMove(x: Float(dx), y: Float(dy)) }
        view.addSubview(leftStick)

        rightStick.frame.origin.x = view.bounds.width - 200
        rightStick.frame.origin.y = view.bounds.height - 200
        rightStick.onChange = { dx, dy in DioniteBridge.shared.setAim(x: Float(dx), y: Float(dy)) }
        view.addSubview(rightStick)

        fireButton.frame = CGRect(x: view.bounds.width - 100, y: view.bounds.height - 320, width: 70, height: 70)
        fireButton.setTitle("FIRE", for: .normal)
        fireButton.backgroundColor = UIColor(red: 0.88, green: 0.11, blue: 0.28, alpha: 0.85)
        fireButton.layer.cornerRadius = 35
        fireButton.addTarget(self, action: #selector(fireDown), for: .touchDown)
        fireButton.addTarget(self, action: #selector(fireUp), for: [.touchUpInside, .touchUpOutside, .touchCancel])
        view.addSubview(fireButton)
    }

    @objc func fireDown() { DioniteBridge.shared.setFire(true) }
    @objc func fireUp()   { DioniteBridge.shared.setFire(false) }

    func buildPipeline() {
        guard let library = device.makeDefaultLibrary() else { return }
        let vfn = library.makeFunction(name: "vertex_main")
        let ffn = library.makeFunction(name: "fragment_main")
        let desc = MTLRenderPipelineDescriptor()
        desc.vertexFunction = vfn
        desc.fragmentFunction = ffn
        desc.colorAttachments[0].pixelFormat = metalView.colorPixelFormat
        desc.depthAttachmentPixelFormat = metalView.depthStencilPixelFormat
        pipelineState = try? device.makeRenderPipelineState(descriptor: desc)
    }

    // MARK: - MTKViewDelegate
    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
        DioniteBridge.shared.resize(width: Int32(size.width), height: Int32(size.height))
    }

    func draw(in view: MTKView) {
        let now = CACurrentMediaTime()
        let dt = Float(now - lastFrameTime)
        lastFrameTime = now

        // Update C++ core for this frame.
        GamepadBridge.shared.pumpToCore()
        DioniteBridge.shared.tick(dt: dt)

        guard let drawable = view.currentDrawable,
              let rpDesc = view.currentRenderPassDescriptor,
              let cmdBuf = commandQueue.makeCommandBuffer(),
              let encoder = cmdBuf.makeRenderCommandEncoder(descriptor: rpDesc),
              let pipe = pipelineState else { return }

        encoder.setRenderPipelineState(pipe)
        // Real impl: encoder.setVertexBuffer(...), drawIndexedPrimitives(...)
        // For scaffolding, we just end encoding to produce a clear color.
        encoder.endEncoding()
        cmdBuf.present(drawable)
        cmdBuf.commit()
    }
}
