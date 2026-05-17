// ============================================================================
// Dionite — iOS virtual joystick (touch UI control)
// ============================================================================
import UIKit

final class VirtualJoystick: UIView {
    private let outer = CAShapeLayer()
    private let knob  = CAShapeLayer()
    private var center0: CGPoint = .zero
    var onChange: ((CGFloat, CGFloat) -> Void)?

    override init(frame: CGRect) {
        super.init(frame: frame)
        backgroundColor = .clear
        center0 = CGPoint(x: bounds.midX, y: bounds.midY)
        let r = bounds.width / 2
        outer.path = UIBezierPath(arcCenter: center0, radius: r, startAngle: 0, endAngle: .pi*2, clockwise: true).cgPath
        outer.fillColor = UIColor(white: 0.05, alpha: 0.4).cgColor
        outer.strokeColor = UIColor(red: 0.65, green: 0.52, blue: 0.30, alpha: 0.7).cgColor
        outer.lineWidth = 2
        layer.addSublayer(outer)
        knob.path = UIBezierPath(arcCenter: center0, radius: r*0.45, startAngle: 0, endAngle: .pi*2, clockwise: true).cgPath
        knob.fillColor = UIColor(red: 0.65, green: 0.52, blue: 0.30, alpha: 0.9).cgColor
        layer.addSublayer(knob)
        isMultipleTouchEnabled = true
    }
    required init?(coder: NSCoder) { fatalError() }

    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) { update(touches) }
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) { update(touches) }
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) { reset() }
    override func touchesCancelled(_ touches: Set<UITouch>, with event: UIEvent?) { reset() }

    private func update(_ touches: Set<UITouch>) {
        guard let t = touches.first else { return }
        let p = t.location(in: self)
        let r = bounds.width / 2
        var dx = p.x - center0.x
        var dy = p.y - center0.y
        let len = sqrt(dx*dx + dy*dy)
        if len > r { dx = dx / len * r; dy = dy / len * r }
        knob.path = UIBezierPath(arcCenter: CGPoint(x: center0.x + dx, y: center0.y + dy), radius: r*0.45, startAngle: 0, endAngle: .pi*2, clockwise: true).cgPath
        onChange?(dx / r, dy / r)
    }
    private func reset() {
        let r = bounds.width / 2
        knob.path = UIBezierPath(arcCenter: center0, radius: r*0.45, startAngle: 0, endAngle: .pi*2, clockwise: true).cgPath
        onChange?(0, 0)
    }
}
