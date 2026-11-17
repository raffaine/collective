import Foundation
import AVFoundation
import CoreGraphics
import CoreText
import AppKit

struct SceneSpec {
    let path: String
    let actLabel: String
    let placeLabel: String
}

// 30 unique eye-level walking scenes across Episode 10 (Wave F Part 2: Psi, Rho, Omega)
let scenes: [SceneSpec] = [
    // Part I: Scenario Psi — Transformative Justice, Unmediated Peace Hearths & Graceful Fission
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_02.png", actLabel: "PART I // SCENARIO PSI: <500MS ABUSE BYPASS", placeLabel: "Immediate Safe-Harbor Sanctuary & Bamboo Pond"),
    SceneSpec(path: "doc/podcast/renders/ep05/scenes/scene_14.png", actLabel: "PART I // SCENARIO PSI: NO-TRANSIT BOUNDARY", placeLabel: "Hardware-Enforced Separation Polygon & Bollard"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_01.png", actLabel: "PART I // SCENARIO PSI: UNMEDIATED PEACE HEARTH", placeLabel: "Open Brass Mic Kill-Switch & 3-of-3 NFC Table"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_06.png", actLabel: "PART I // SCENARIO PSI: 24H COOLING-OFF BUFFER", placeLabel: "Tea Veranda & VRF Sortition Mediator Circle"),
    SceneSpec(path: "doc/podcast/renders/ep05/scenes/scene_07.png", actLabel: "PART I // SCENARIO PSI: SYSTEMIC ROOT-CAUSE FIX", placeLabel: "Upgrading Bottlenecked Shop Tooling & Ventilation"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_03.png", actLabel: "PART I // SCENARIO PSI: WATERSHED STANDING (F_ECO)", placeLabel: "Riparian Willow Restitution & Creek Restoration"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_02.png", actLabel: "PART I // SCENARIO PSI: ECOLOGICAL ADVOCACY", placeLabel: "Protected Mother-Tree Grove & T1 Mentored Step-Down"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_04.png", actLabel: "PART I // SCENARIO PSI: GRACEFUL FISSION ARCHITECT", placeLabel: "Envy-Free Proportional Tool & Seed Partition"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_05.png", actLabel: "PART I // SCENARIO PSI: PEACEFUL SISTER FORK", placeLabel: "Two Diverging Woodland Trails & Federated Valleys"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_13.png", actLabel: "PART I // SCENARIO PSI: FAA ARBITRATION SHIELD", placeLabel: "Inter-Valley Stone Bridge & Zero Carceral Cages"),

    // Part II: Scenario Rho — Autopoietic Immunity, Fault-Before-Malice & Subpoena Sterility
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_06.png", actLabel: "PART II // SCENARIO RHO: FAULT BEFORE MALICE", placeLabel: "Two-Person Flowmeter & Pipe Union Leak Check"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_07.png", actLabel: "PART II // SCENARIO RHO: CONSERVATION CALIBRATION", placeLabel: "Manganin Shunt & Reference Metrology Bench"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_08.png", actLabel: "PART II // SCENARIO RHO: VLAN 40 SPLIT-SANDBOX", placeLabel: "Optical Diode Partition & Prompt Quarantine"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_02.png", actLabel: "PART II // SCENARIO RHO: HARDWARE AIR-GAP", placeLabel: "Physical Brass VLAN Selector & Zero Tool Authority"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_07.png", actLabel: "PART II // SCENARIO RHO: 0 BP SYBIL BOT WEIGHT", placeLabel: "In-Person 40mm NFC Co-Presence Web of Trust"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_09.png", actLabel: "PART II // SCENARIO RHO: NAACP V. ALABAMA STERILITY", placeLabel: "Empty Glass Ledger Case (Zero Stored Member Data)"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_11.png", actLabel: "PART II // SCENARIO RHO: VRF ANTI-CARTEL SORTITION", placeLabel: "Randomized Oversight & k>=3 Guild Redundancy"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_07.png", actLabel: "PART II // SCENARIO RHO: 72-HOUR SECURITY SUNSET", placeLabel: "Anti-Autoimmune Guardrail & 10,000 bp Floor"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_10.png", actLabel: "PART II // SCENARIO RHO: MULTI-BANK FAILOVER", placeLabel: "Resilient Coastal Seawall & Tripartite Carapace"),

    // Part III: Scenario Omega — Terminal Decoupling, 15-Hour Weeks & The Planetary Lighthouse
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_11.png", actLabel: "PART III // SCENARIO OMEGA: 5-DOMAIN OBSERVATORY", placeLabel: "All Five Domains <=200 bp Across 12 Lunar Cycles"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_01.png", actLabel: "PART III // SCENARIO OMEGA: 100% TAX ENDOWMENT", placeLabel: "Conservation Easement & Perpetual Purpose Trust"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_15.png", actLabel: "PART III // SCENARIO OMEGA: ESTATE LIBERATION", placeLabel: "Memorial Coppice Grove & Wisdom Anchor Continuity"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_11.png", actLabel: "PART III // SCENARIO OMEGA: 80% ENTRENCHED CORE", placeLabel: "60-Day Islanded Drill & Lawful Carapace Sunset"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_12.png", actLabel: "PART III // SCENARIO OMEGA: <100MS ZEROIZATION", placeLabel: "Open Utility Intertie Monument Under Glass"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_13.png", actLabel: "PART III // SCENARIO OMEGA: <=15 HOURS A WEEK", placeLabel: "Orchard Easel, Acoustic Guitar & Post-Scarcity Noon"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_13.png", actLabel: "PART III // SCENARIO OMEGA: CALM PERMANENCE", placeLabel: "2,600 Workflows Coalesced into 3 Daily Rounds"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_15.png", actLabel: "PART III // SCENARIO OMEGA: NO BUNKER ISOLATION", placeLabel: "Open-Armed Bioregional Commonwealth at Dusk"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_14.png", actLabel: "PART III // SCENARIO OMEGA: >=5% GENESIS SEEDBEDS", placeLabel: "Solar & Mother-Machine Seed Kits on Wharf Barge"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_15.png", actLabel: "PART III // SCENARIO OMEGA: PLANETARY LIGHTHOUSE", placeLabel: "R6 Optical Beacon Shining Across the Horizon"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_15.png", actLabel: "NEXT // EPISODE 11: THE SEASON 1 GRAND FINALE", placeLabel: "The Minimum Viable Society & The Historical Rupture")
]

let width = 1920
let height = 1080
let fps: Int32 = 8
let crossfadeSec: Double = 5.0
let colorSpace = CGColorSpaceCreateDeviceRGB()
let bitmapInfo = CGImageAlphaInfo.premultipliedFirst.rawValue | CGBitmapInfo.byteOrder32Little.rawValue

func loadNormalizedCGImage(_ path: String) -> CGImage {
    guard let nsImg = NSImage(contentsOfFile: path),
          let srcCG = nsImg.cgImage(forProposedRect: nil, context: nil, hints: nil) else {
        fatalError("Failed to load image at \(path)")
    }
    guard let ctx = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: 8,
        bytesPerRow: width * 4,
        space: colorSpace,
        bitmapInfo: bitmapInfo
    ) else {
        fatalError("Cannot create normalize CGContext")
    }
    ctx.interpolationQuality = .high
    let scaleX = CGFloat(width) / CGFloat(srcCG.width)
    let scaleY = CGFloat(height) / CGFloat(srcCG.height)
    let scale = max(scaleX, scaleY)
    let drawW = CGFloat(srcCG.width) * scale
    let drawH = CGFloat(srcCG.height) * scale
    let dx = (CGFloat(width) - drawW) * 0.5
    let dy = (CGFloat(height) - drawH) * 0.5
    ctx.draw(srcCG, in: CGRect(x: dx, y: dy, width: drawW, height: drawH))
    return ctx.makeImage()!
}

func makeHudCGImage(actLabel: String, placeLabel: String) -> CGImage {
    guard let ctx = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: 8,
        bytesPerRow: width * 4,
        space: colorSpace,
        bitmapInfo: bitmapInfo
    ) else {
        fatalError("Cannot create HUD CGContext")
    }

    let boxRect = CGRect(x: 48, y: 42, width: 760, height: 86)
    let boxPath = CGPath(roundedRect: boxRect, cornerWidth: 8, cornerHeight: 8, transform: nil)
    ctx.setFillColor(CGColor(red: 0.06, green: 0.08, blue: 0.10, alpha: 0.62))
    ctx.addPath(boxPath)
    ctx.fillPath()

    ctx.setStrokeColor(CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.45))
    ctx.setLineWidth(1.5)
    ctx.addPath(boxPath)
    ctx.strokePath()

    let pillRect = CGRect(x: 48, y: height - 66, width: 660, height: 34)
    let pillPath = CGPath(roundedRect: pillRect, cornerWidth: 6, cornerHeight: 6, transform: nil)
    ctx.setFillColor(CGColor(red: 0.06, green: 0.08, blue: 0.10, alpha: 0.55))
    ctx.addPath(pillPath)
    ctx.fillPath()

    func drawText(_ text: String, font: CTFont, color: CGColor, x: CGFloat, y: CGFloat) {
        let attrs: [NSAttributedString.Key: Any] = [
            NSAttributedString.Key(kCTFontAttributeName as String): font,
            NSAttributedString.Key(kCTForegroundColorAttributeName as String): color
        ]
        let attrStr = NSAttributedString(string: text, attributes: attrs)
        let line = CTLineCreateWithAttributedString(attrStr)
        ctx.textPosition = CGPoint(x: x, y: y)
        CTLineDraw(line, ctx)
    }

    let fontTop = CTFontCreateWithName("Courier-Bold" as CFString, 15, nil)
    let fontAct = CTFontCreateWithName("Courier-Bold" as CFString, 16, nil)
    let fontPlace = CTFontCreateWithName("Helvetica-Bold" as CFString, 23, nil)

    let gold = CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.95)
    let green = CGColor(red: 0.48, green: 0.82, blue: 0.42, alpha: 0.95)
    let white = CGColor(red: 0.95, green: 0.96, blue: 0.98, alpha: 0.96)

    drawText("RADIO LAMBDA // EP. 10: WAVE F (PART 2) — PSI, RHO & OMEGA", font: fontTop, color: gold, x: 64, y: CGFloat(height - 54))
    drawText(actLabel, font: fontAct, color: green, x: 68, y: 96)
    drawText(placeLabel, font: fontPlace, color: white, x: 68, y: 62)

    return ctx.makeImage()!
}

let semaphore = DispatchSemaphore(value: 0)

Task {
    do {
        let audioPath = "doc/podcast/episodes/10.episode.m4a"
        let finalOutPath = "doc/podcast/episodes/10.episode_youtube.mp4"
        let tempVideoPath = "/tmp/ep10_silent_video.mp4"

        try? FileManager.default.removeItem(atPath: tempVideoPath)
        try? FileManager.default.removeItem(atPath: finalOutPath)

        let audioURL = URL(fileURLWithPath: audioPath)
        let audioAsset = AVURLAsset(url: audioURL)
        let durationCM = try await audioAsset.load(.duration)
        let durationSec = CMTimeGetSeconds(durationCM)
        print(String(format: "Audio duration: %.2f seconds (%.2f minutes)", durationSec, durationSec / 60.0))

        let cgScenes = scenes.map { loadNormalizedCGImage($0.path) }
        let hudScenes = scenes.map { makeHudCGImage(actLabel: $0.actLabel, placeLabel: $0.placeLabel) }
        let sceneDuration = durationSec / Double(scenes.count)

        let writer = try AVAssetWriter(outputURL: URL(fileURLWithPath: tempVideoPath), fileType: .mp4)
        let videoSettings: [String: Any] = [
            AVVideoCodecKey: AVVideoCodecType.h264,
            AVVideoWidthKey: width,
            AVVideoHeightKey: height,
            AVVideoCompressionPropertiesKey: [
                AVVideoAverageBitRateKey: 1_800_000,
                AVVideoMaxKeyFrameIntervalKey: 16,
                AVVideoProfileLevelKey: AVVideoProfileLevelH264HighAutoLevel
            ]
        ]
        let writerInput = AVAssetWriterInput(mediaType: .video, outputSettings: videoSettings)
        writerInput.expectsMediaDataInRealTime = false

        let pixelBufferAttrs: [String: Any] = [
            kCVPixelBufferPixelFormatTypeKey as String: Int(kCVPixelFormatType_32BGRA),
            kCVPixelBufferWidthKey as String: width,
            kCVPixelBufferHeightKey as String: height,
            kCVPixelBufferCGImageCompatibilityKey as String: true,
            kCVPixelBufferCGBitmapContextCompatibilityKey as String: true
        ]
        let adaptor = AVAssetWriterInputPixelBufferAdaptor(
            assetWriterInput: writerInput,
            sourcePixelBufferAttributes: pixelBufferAttrs
        )

        writer.add(writerInput)
        writer.startWriting()
        writer.startSession(atSourceTime: .zero)

        let totalFrames = Int(ceil(durationSec * Double(fps)))
        print("Rendering \(totalFrames) frames at \(fps) fps across \(scenes.count) scenes...")

        func drawSceneKenBurns(_ ctx: CGContext, img: CGImage, index: Int, progress: Double, alpha: CGFloat) {
            let p = CGFloat(max(0.0, min(1.0, progress)))
            let zoomIn = (index % 2 == 0)
            let scale: CGFloat = zoomIn ? (1.02 + 0.08 * p) : (1.10 - 0.08 * p)
            let panDir: CGFloat = (index % 3 == 0) ? 1.0 : ((index % 3 == 1) ? -1.0 : 0.4)
            let maxPanX = CGFloat(width) * 0.025
            let maxPanY = CGFloat(height) * 0.015
            let offsetX = panDir * maxPanX * (p - 0.5)
            let offsetY = -panDir * maxPanY * (p - 0.5)

            let drawW = CGFloat(width) * scale
            let drawH = CGFloat(height) * scale
            let x = (CGFloat(width) - drawW) * 0.5 + offsetX
            let y = (CGFloat(height) - drawH) * 0.5 + offsetY

            ctx.saveGState()
            if alpha < 0.999 {
                ctx.setAlpha(alpha)
            }
            ctx.interpolationQuality = .low
            ctx.draw(img, in: CGRect(x: x, y: y, width: drawW, height: drawH))
            ctx.restoreGState()
        }

        for frameIdx in 0..<totalFrames {
            while !writerInput.isReadyForMoreMediaData {
                Thread.sleep(forTimeInterval: 0.002)
            }

            autoreleasepool {
                let t = Double(frameIdx) / Double(fps)
                let rawIdx = Int(t / sceneDuration)
                let curIdx = min( max(0, rawIdx), scenes.count - 1 )
                let sceneLocalT = t - Double(curIdx) * sceneDuration
                let prog = sceneLocalT / sceneDuration

                var pixelBufferOut: CVPixelBuffer?
                CVPixelBufferPoolCreatePixelBuffer(nil, adaptor.pixelBufferPool!, &pixelBufferOut)
                guard let pixelBuffer = pixelBufferOut else { return }

                CVPixelBufferLockBaseAddress(pixelBuffer, [])
                let baseAddr = CVPixelBufferGetBaseAddress(pixelBuffer)!
                let bytesPerRow = CVPixelBufferGetBytesPerRow(pixelBuffer)

                let ctx = CGContext(
                    data: baseAddr,
                    width: width,
                    height: height,
                    bitsPerComponent: 8,
                    bytesPerRow: bytesPerRow,
                    space: colorSpace,
                    bitmapInfo: bitmapInfo
                )!

                drawSceneKenBurns(ctx, img: cgScenes[curIdx], index: curIdx, progress: prog, alpha: 1.0)

                if curIdx + 1 < scenes.count && (sceneDuration - sceneLocalT) < crossfadeSec {
                    let fadeAlpha = CGFloat((crossfadeSec - (sceneDuration - sceneLocalT)) / crossfadeSec)
                    drawSceneKenBurns(ctx, img: cgScenes[curIdx + 1], index: curIdx + 1, progress: 0.0, alpha: fadeAlpha)
                }

                ctx.interpolationQuality = .none
                ctx.draw(hudScenes[curIdx], in: CGRect(x: 0, y: 0, width: width, height: height))

                let totalProg = CGFloat(t / durationSec)
                ctx.setFillColor(CGColor(red: 1.0, green: 1.0, blue: 1.0, alpha: 0.18))
                ctx.fill(CGRect(x: 0, y: 0, width: CGFloat(width), height: 6))
                ctx.setFillColor(CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.92))
                ctx.fill(CGRect(x: 0, y: 0, width: CGFloat(width) * totalProg, height: 6))

                CVPixelBufferUnlockBaseAddress(pixelBuffer, [])

                let pts = CMTime(value: CMTimeValue(frameIdx), timescale: fps)
                adaptor.append(pixelBuffer, withPresentationTime: pts)
            }

            if frameIdx % 1920 == 0 && frameIdx > 0 {
                let pct = Int((Double(frameIdx) / Double(totalFrames)) * 100.0)
                print("Progress: \(pct)% (\(frameIdx)/\(totalFrames) frames)")
            }
        }

        writerInput.markAsFinished()
        await writer.finishWriting()
        print("Silent video track rendered. Now muxing audio losslessly...")

        let composition = AVMutableComposition()
        let videoAsset = AVURLAsset(url: URL(fileURLWithPath: tempVideoPath))

        let compVideoTrack = composition.addMutableTrack(withMediaType: .video, preferredTrackID: kCMPersistentTrackID_Invalid)!
        let srcVideoTrack = try await videoAsset.loadTracks(withMediaType: .video).first!
        try compVideoTrack.insertTimeRange(CMTimeRange(start: .zero, duration: durationCM), of: srcVideoTrack, at: .zero)

        let compAudioTrack = composition.addMutableTrack(withMediaType: .audio, preferredTrackID: kCMPersistentTrackID_Invalid)!
        let srcAudioTrack = try await audioAsset.loadTracks(withMediaType: .audio).first!
        try compAudioTrack.insertTimeRange(CMTimeRange(start: .zero, duration: durationCM), of: srcAudioTrack, at: .zero)

        let exporter = AVAssetExportSession(asset: composition, presetName: AVAssetExportPresetPassthrough)!
        exporter.outputURL = URL(fileURLWithPath: finalOutPath)
        exporter.outputFileType = .mp4
        await exporter.export()

        try? FileManager.default.removeItem(atPath: tempVideoPath)
        print("SUCCESS: Created YouTube video at \(finalOutPath)")
        semaphore.signal()
    } catch {
        print("ERROR: \(error)")
        exit(1)
    }
}

semaphore.wait()
